/* /////////////////////////////////////////////////////////////////////////////
//   _
//  | |_ _ __ ___  ___  ___   treee: an interactive file tree viewer
//  | __| '__/ _ \/ _ \/ _ \  Copyright (C) 2020-2023 Justin Collier
//  | |_| | |  __/  __/  __/
//   \__|_|  \___|\___|\___|  - - - - - - - - - - - - - - - - - -
//
//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//                                                                             /
//  You should have received a copy of the GNU General Public License         //
//  along with this program.  If not, see <https://www.gnu.org/licenses/>.   ///
///////////////////////////////////////////////////////////////////////////// */

#include <algorithm>
#include <iostream>
#include <iterator>
#include <string>

#include "treee/root.h"

#ifndef TREEE_NO_GIT
#include "treee/git.h"
static treee::git g_git;
#endif

namespace treee {

bool
root::file::operator==(const file &other) const {
  return path == other.path && is_last_sibling == other.is_last_sibling &&
         is_directory == other.is_directory && is_symlink == other.is_symlink &&
         depth == other.depth;
}

root::root(fs::path p) : path_(p) {
}

const fs::path &
root::path() const noexcept {
  return path_;
}
const vector<root::file> &
root::files() const noexcept {
  return files_;
}
int
root::max_depth() const noexcept {
  return max_depth_;
}
bool
root::stopped_early() const noexcept {
  return stopped_early_;
}

struct update_state {
  const fs::path &root_path;
  vector<root::file>::const_iterator verifier;
  vector<root::file> &files;
  const settings &s;
  bool found_diff;
  char pad_[7]{};
  int max_depth{-1};
  bool stopped_early{false};
};

static void
process_directory_entry(
    update_state &o, const fs::directory_entry &entry, bool is_last_sibling,
    int depth = 0) {
  // construct a candidate file object
  root::file candidate{};
  candidate.path            = entry.path();
  candidate.is_last_sibling = is_last_sibling;
  candidate.is_directory    = entry.is_directory();
  candidate.is_symlink      = entry.is_symlink();
  candidate.depth           = depth;
  // if a diff was previously found,
  if (o.found_diff) {
    // push back the candidate
    o.files.push_back(candidate);
  }
  // else if verify_it has ended before loop completion,
  else if (o.verifier == o.files.end()) {
    // mark found_diff and continue
    o.found_diff = true;
    return;
  }
  // else if the candidate matches the verifier,
  else if (*o.verifier == candidate) {
    // increment the verifier and continue
    ++o.verifier;
    return;
  }
  // else, mark diff, erase remaining portion of files_, and push candidate
  else {
    o.found_diff = true;
    o.files.erase(o.verifier, o.files.end());
    o.files.push_back(candidate);
  }
}

static void
recurse_process_directories(
    update_state &o, const fs::path &cur_dir, int depth = 0) {
  if (o.s.max_depth >= 0 && depth > o.s.max_depth) {
    o.stopped_early = true;
    return;
  }
  if (depth > o.max_depth)
    o.max_depth = depth;
  try {
    auto children = vector<fs::directory_entry>{};
    {
      auto it = fs::directory_iterator{cur_dir};
      std::copy_if(
          fs::begin(it), fs::end(it), std::back_inserter(children),
          [&o](auto &&v) {
            if (!o.s.read_hidden && v.path().filename().c_str()[0] == '.' &&
                v.path() != fs::path{"."})
              return false;
#ifndef TREEE_NO_GIT
            if (!o.s.read_gitignored && g_git.is_ignored(o.root_path, v.path()))
              return false;
#endif
            if (!o.s.read_patignored)
              for (auto &&r : o.s.ignore_pats)
                if (std::regex_match(v.path().filename().string(), r))
                  return false;

            return true;
          });
      std::sort(children.begin(), children.end());
    }

    for (auto it = children.begin(); it != children.end(); ++it) {
      process_directory_entry(o, *it, next(it) == children.end(), depth);
      if (it->is_directory() && !it->is_symlink())
        recurse_process_directories(o, it->path(), depth + 1);
    }
  } catch (...) {}
}

bool
root::update(settings &s) {
  update_state state{
      /* .root_path  = */ path_,
      /* .verifier   = */ files_.cbegin(),
      /* .files      = */ files_,
      /* .s          = */ s,
      /* .found_diff = */ files_.empty(),
  };

  recurse_process_directories(state, path_);

  // if a diff was not found but the verifier had not ended with the loop,
  if (!state.found_diff and state.verifier != files_.end()) {
    // mark diff and erase the remaining paths
    state.found_diff = true;
    files_.erase(state.verifier, files_.end());
  }

  max_depth_     = state.max_depth;
  stopped_early_ = state.stopped_early;

  // returns true if any elements of files_ have changed
  return state.found_diff;
}

} // namespace treee

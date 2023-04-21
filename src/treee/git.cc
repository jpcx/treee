/* /////////////////////////////////////////////////////////////////////////////
//   _
//  | |_ _ __ ___  ___  ___   treee: an interactive file tree viewer
//  | __| '__/ _ \/ _ \/ _ \  Copyright (C) 2020 Justin Collier
//  | |_| | |  __/  __/  __/
//   \__|_|  \___|\___|\___|  - - - - - - - - - - - - - - - - - -
//
//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 2 of the License, or
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

#ifndef TREEE_NO_GIT

#include <iostream>
#include <utility>

#include "git.h"

extern "C" {
#include <git2.h>
}

namespace treee {

static bool g_did_instantiate = false;

git::git() {
  if (g_did_instantiate) {
    std::cerr << "[treee] Internal Error: attempted to create more than one "
                 "git object\n";
    std::terminate();
  }
  g_did_instantiate = true;
  git_libgit2_init();
}
git::~git() {
  git_libgit2_shutdown();
}

bool
git::is_ignored(const fs::path &repo, const fs::path &path) const noexcept {
  if (repo.string().size() == 0)
    return false;
  auto &&[it, x] = repos_.emplace(std::make_pair(repo, repo));
  if (path.string().size() <= 2)
    return false;
  if (it->second.repo() == nullptr) {
    repos_.erase(it);
    return false;
  }
  int ignored = 0;
  git_status_should_ignore(
      &ignored, reinterpret_cast<git_repository *>(it->second.repo()),
      &path.c_str()[2]);
  return !!ignored;
}

git::repository::repository(const fs::path &path) {
  git_repository *repo;
  int result = git_repository_open(&repo, path.c_str());
  if (result)
    repo_ = nullptr;
  else
    repo_ = reinterpret_cast<void *>(repo);
}
git::repository::~repository() {
  git_repository_free(reinterpret_cast<git_repository *>(repo_));
}
void *
git::repository::repo() const noexcept {
  return repo_;
}

} // namespace treee

#endif

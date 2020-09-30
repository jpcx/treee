#ifndef TREEE_ROOT_H_INCLUDED
#define TREEE_ROOT_H_INCLUDED
/* /////////////////////////////////////////////////////////////////////////////
//   _
//  | |_ _ __ ___  ___  ___   treee: an interactive file tree viewer
//  | __| '__/ _ \/ _ \/ _ \  Copyright (C) 2020 Justin Collier
//  | |_| | |  __/  __/  __/
//   \__|_|  \___|\___|\___|  - - - - - - - - - - - - - - - - - -
//
//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the internalied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//                                                                             /
//  You should have received a copy of the GNU General Public License         //
//  along with this program.  If not, see <https://www.gnu.org/licenses/>.   ///
///////////////////////////////////////////////////////////////////////////// */

#include <filesystem>
#include <mutex>
#include <vector>

#include "treee/settings.h"

namespace treee {

using std::vector;

namespace fs = std::filesystem;

class root {
 public:
  struct file {
    fs::path path;
    bool is_last_sibling;
    bool is_directory;
    bool is_symlink;
    char pad_[1]{};
    int depth;
    bool
    operator==(const file &) const;
  };

  root(fs::path);
  root()             = delete;
  root(const root &) = delete;
  root(root &&)      = default;
  ~root()            = default;

  [[nodiscard]] const fs::path &
  path() const noexcept;

  [[nodiscard]] const vector<file> &
  files() const noexcept;

  bool
  update(settings &s);

 private:
  fs::path path_;
  vector<file> files_{};
};

} // namespace treee

#endif

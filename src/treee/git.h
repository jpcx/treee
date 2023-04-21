#ifndef TREEE_GIT_H_INCLUDED
#define TREEE_GIT_H_INCLUDED
/* /////////////////////////////////////////////////////////////////////////////
//   _
//  | |_ _ __ ___  ___  ___   treee: an interactive file tree viewer
//  | __| '__/ _ \/ _ \/ _ \  Copyright (C) 2020-2023 Justin Collier
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

#include <compare>
#include <filesystem>
#include <map>

namespace treee {

namespace fs = std::filesystem;

using std::map;

class git {
 public:
  git();
  ~git();

  [[nodiscard]] bool
  is_ignored(const fs::path &repo, const fs::path &path) const noexcept;

 private:
  class repository {
   public:
    repository(const fs::path &);
    ~repository();

    [[nodiscard]] void *
    repo() const noexcept;

   private:
    void *repo_;
  };

  mutable map<fs::path, repository> repos_;
};

} // namespace treee

#endif // #ifndef TREEE_NO_GIT

#endif

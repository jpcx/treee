#ifndef TREEE_SETTINGS_H_INCLUDED
#define TREEE_SETTINGS_H_INCLUDED
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
//    but WITHOUT ANY WARRANTY; without even the internalied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//                                                                             /
//  You should have received a copy of the GNU General Public License         //
//  along with this program.  If not, see <https://www.gnu.org/licenses/>.   ///
///////////////////////////////////////////////////////////////////////////// */

#include <atomic>
#include <mutex>
#include <regex>
#include <vector>

namespace treee {

struct settings {
  std::vector<std::regex> ignore_pats{};
  std::mutex roots_access_mutex;
  std::atomic<bool> read_hidden     = false;
  std::atomic<bool> read_patignored = false;
#ifndef TREEE_NO_GIT
  std::atomic<bool> read_gitignored = false;
#endif
  char pad_[5];
};

} // namespace treee

#endif

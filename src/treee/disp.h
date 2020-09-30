#ifndef TREEE_DISP_H_INCLUDED
#define TREEE_DISP_H_INCLUDED
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

#include <string>

#include "treee/root.h"
#include "treee/settings.h"

namespace treee {

using std::string;

class disp {
 public:
  disp()             = default;
  disp(const disp &) = delete;
  disp(disp &&)      = delete;
  ~disp()            = default;

  void
  repaint(settings &s, const vector<root> &roots);
  bool
  check_input(settings &s, vector<root> &roots);

 private:
  int winx_{0};
  int winy_{0};
  int view_cols_{0};
  int lines_{0};
};

} // namespace treee

#endif

#ifndef TREEE_DISP_H_INCLUDED
#define TREEE_DISP_H_INCLUDED
/* /////////////////////////////////////////////////////////////////////////////
//   _
//  | |_ _ __ ___  ___  ___   treee: interactive file tree viewer [0.1.0]
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

#include "file.h"

#define PAIR_CYAN    1
#define PAIR_MAGENTA 2
#define PAIR_GREEN   3
#define PAIR_ERROR   4

typedef struct window {
  long x;
  long y;
  int showhidden;
} view;

void
printtree(file *node, long *ln, view *win, long *max_x);

void
initcurses();

void
destroycurses();

#endif

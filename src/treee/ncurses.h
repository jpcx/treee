#ifndef TREEE_NCURSES_H_INCLUDED
#define TREEE_NCURSES_H_INCLUDED
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

#define PAIR_CYAN  1
#define PAIR_BLACK 2
#define PAIR_GREEN 3
#define PAIR_ERROR 4

namespace treee {

namespace detail_ {
struct LINES {
  operator int();
};
struct A_BOLD_ {
  operator unsigned();
};
struct ACS_VLINE_ {
  operator unsigned();
};
struct ACS_LLCORNER_ {
  operator unsigned();
};
struct ACS_LTEE_ {
  operator unsigned();
};
struct COLOR_PAIR {
  unsigned
  operator()(short pair);
};
struct attron {
  void
  operator()(unsigned attr);
};
struct attroff {
  void
  operator()(unsigned attr);
};
struct mvaddch {
  void
  operator()(int x, int y, unsigned ch);
};
struct mvaddstr {
  void
  operator()(int x, int y, const char *str);
};
struct timeout {
  void
  operator()(int ms);
};
struct getch {
  int
  operator()();
};
struct clear {
  void
  operator()();
};
} // namespace detail_

#ifndef __NCURSES_H
inline detail_::LINES LINES;
inline detail_::A_BOLD_ A_BOLD;
inline detail_::ACS_VLINE_ ACS_VLINE;
inline detail_::ACS_LLCORNER_ ACS_LLCORNER;
inline detail_::ACS_LTEE_ ACS_LTEE;
inline detail_::COLOR_PAIR COLOR_PAIR;
inline detail_::attron attron;
inline detail_::attroff attroff;
inline detail_::mvaddch mvaddch;
inline detail_::mvaddstr mvaddstr;
inline detail_::timeout timeout;
inline detail_::getch getch;
inline detail_::clear clear;
#endif

} // namespace treee

#endif

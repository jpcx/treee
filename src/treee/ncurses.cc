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

extern "C" {
#include <ncurses.h>
}

#include "treee/ncurses.h"

namespace treee {

void
ncurses_init() {
  initscr();
  cbreak();
  noecho();
  nodelay(stdscr, TRUE);
  keypad(stdscr, TRUE);
  clear();
  if (has_colors()) {
    start_color();
    use_default_colors();
    init_pair(PAIR_CYAN, COLOR_CYAN, -1);
    init_pair(PAIR_BLACK, COLOR_BLACK, -1);
    init_pair(PAIR_GREEN, COLOR_GREEN, -1);
    init_pair(PAIR_ERROR, COLOR_WHITE, COLOR_RED);
  }
}

void
ncurses_cleanup() {
  nocbreak();
  echo();
  nodelay(stdscr, FALSE);
  keypad(stdscr, FALSE);
  endwin();
}

detail_::LINES::operator int() {
  return ::LINES;
}

detail_::COLS::operator int() {
  return ::COLS;
}

detail_::A_BOLD_::operator unsigned() {
  return A_BOLD;
}

detail_::ACS_VLINE_::operator unsigned() {
  return ACS_VLINE;
}

detail_::ACS_LLCORNER_::operator unsigned() {
  return ACS_LLCORNER;
}

detail_::ACS_LTEE_::operator unsigned() {
  return ACS_LTEE;
}

unsigned
detail_::COLOR_PAIR::operator()(short pair) {
  return COLOR_PAIR(pair);
}

void
detail_::attron::operator()(unsigned attr) {
  attron(attr);
}

void
detail_::attroff::operator()(unsigned attr) {
  attroff(attr);
}

void
detail_::mvaddch::operator()(int y, int x, unsigned ch) {
  mvaddch(y, x, ch);
}

void
detail_::mvaddstr::operator()(int y, int x, const char *str) {
  mvaddstr(y, x, str);
}

void
detail_::timeout::operator()(int ms) {
  timeout(ms);
}

int
detail_::getch::operator()() {
  return getch();
}

void
detail_::clear::operator()() {
  ::clear();
}

} // namespace treee

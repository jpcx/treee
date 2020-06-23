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

#include <ncurses.h>

#include "disp.h"

static void
mvaddstr_bounded(long y, long x, const char *text, size_t len, long *max_x) {
  if (x >= 0) {
    mvaddstr(y, x, text);
  } else if ((-x + 2) < len) {
    mvaddch(y, 0, '<');
    mvaddstr(y, 2, &text[-x + 2]);
  }
}

void
printtree(file *node, long *ln, view *vw, long *max_x) {
  long basex = node->level * 2 - vw->x;
  long basey = *ln - vw->y;
  if (node->name[0] == '.' && node->name[1] != '\0' && !vw->showhidden)
    return;
  if (basey >= 0 && basey <= LINES && basex <= COLS) {
    attron(COLOR_PAIR(PAIR_MAGENTA));
    if (file_has_siblings(node) && strcmp(node->path, ".") != 0) {
      if (file_is_last_sibling(node)) {
        mvaddch(basey, basex - 2, ACS_LLCORNER); // └
      } else {
        mvaddch(basey, basex - 2, ACS_LTEE); // ├
      }
    } else if (node->parent != NULL)
    // only child
    {
      mvaddch(basey, basex - 2, ACS_LLCORNER); // └
    }

    {
      int x = basex - 2;
      for (file *f = node->parent; f != NULL; f = f->parent) {
        x -= 2;
        if (!file_is_last_sibling(f)) {
          mvaddch(basey, x, ACS_VLINE);
        }
      }
    }

    attroff(COLOR_PAIR(PAIR_MAGENTA));

    if (node->is_dir && !node->is_lnk) {
      attron(COLOR_PAIR(PAIR_GREEN));
      mvaddstr_bounded(basey, basex, node->name, str_len(node->name), max_x);
      if (node->level * 2 + str_len(node->name) > *max_x)
        *max_x = node->level * 2 + str_len(node->name);
      attroff(COLOR_PAIR(PAIR_GREEN));
    } else {
      if (node->is_lnk) {
        attron(COLOR_PAIR(PAIR_CYAN));
        mvaddstr_bounded(basey, basex, node->name, str_len(node->name), max_x);
        attroff(COLOR_PAIR(PAIR_CYAN));
        mvaddstr_bounded(
            basey, basex + str_len(node->name) + 1, "->", 2, max_x);
        if (!node->is_valid_lnk) {
          attron(COLOR_PAIR(PAIR_ERROR));
          attron(A_BOLD);
        }
        mvaddstr_bounded(
            basey, basex + str_len(node->name) + 4, node->symlink_target,
            str_len(node->symlink_target), max_x);
        if (!node->is_valid_lnk) {
          attroff(COLOR_PAIR(PAIR_ERROR));
          attroff(A_BOLD);
        }
        if (node->level * 2 + str_len(node->name) + 4 +
                str_len(node->symlink_target) >
            *max_x)
          *max_x = node->level * 2 + str_len(node->name) + 4 +
                   str_len(node->symlink_target);
      } else {
        mvaddstr_bounded(basey, basex, node->name, str_len(node->name), max_x);
        if (node->level * 2 + str_len(node->name) > *max_x)
          *max_x = node->level * 2 + str_len(node->name);
      }
    }
  }

  ++(*ln);
  for (file *f = node->first_child; f != NULL; f = f->next_sibling) {
    printtree(f, ln, vw, max_x);
  }
}

void
initcurses() {
  initscr();
  cbreak();
  noecho();
  clear();
  start_color();
  use_default_colors();
  init_pair(PAIR_CYAN, COLOR_CYAN, -1);
  init_pair(PAIR_MAGENTA, COLOR_MAGENTA, -1);
  init_pair(PAIR_GREEN, COLOR_GREEN, -1);
  init_pair(PAIR_ERROR, COLOR_WHITE, COLOR_RED);
}

void
destroycurses() {
  clear();
  endwin();
}

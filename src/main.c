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
#include "file.h"

int
main(int argc, char *argv[]) {
  initcurses();

  const char *searchdir = (argc < 2 ? "." : argv[1]);

  view vw = {
      .x          = 0,
      .y          = 0,
      .showhidden = 0,
  };

  while (1) {
    clear();
    tinydir_dir dir;
    tinydir_open(&dir, searchdir);

    file *root = file_new(NULL, searchdir, 1, 0);

    file_attach(root, &dir);
    long lines = 0;
    long max_x = 0;
    printtree(root, &lines, &vw, &max_x);
    if (max_x < vw.x + 3) {
      if (max_x >= 3)
        vw.x = max_x - 3;
      else
        vw.x = 0;
      continue;
    }

    cbreak();

    timeout(1000);
    int c = getch();

    switch (c) {
    case 'j': {
      if (lines < LINES)
        break;
      if (vw.y + 1 < lines - LINES + 1)
        vw.y += 1;
      break;
    }
    case 'J': {
      if (lines < LINES)
        break;
      if (vw.y + 10 < lines - LINES + 1)
        vw.y += 10;
      else if (vw.y + 10 >= lines - LINES + 1)
        vw.y = lines - LINES;
      break;
    }
    case 'k': {
      if (vw.y - 1 >= 0)
        vw.y -= 1;
      break;
    }
    case 'K': {
      if (vw.y - 10 >= 0)
        vw.y -= 10;
      else if (vw.y < 10)
        vw.y = 0;
      break;
    }
    case 'l': {
      if (vw.x + 3 < max_x)
        vw.x += 1;
      break;
    }
    case 'L': {
      if (vw.x + 12 < max_x)
        vw.x += 10;
      else if (vw.x + 12 >= max_x)
        vw.x = max_x - 2;
      break;
    }
    case 'h': {
      if (vw.x - 1 >= 0)
        vw.x -= 1;
      break;
    }
    case 'H': {
      if (vw.x - 10 >= 0)
        vw.x -= 10;
      else if (vw.x < 10)
        vw.x = 0;
      break;
    }
    case 'q': {
      destroycurses();
      file_destruct(root);
      tinydir_close(&dir);
      return EXIT_SUCCESS;
      break;
    }
    case '.': {
      vw.showhidden = !vw.showhidden;
      break;
    }
    }

    file_destruct(root);
    tinydir_close(&dir);
  }
}

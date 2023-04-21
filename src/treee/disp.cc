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

#include <mutex>
#include <set>

#include "treee/disp.h"
#include "treee/ncurses.h"

namespace treee {

using std::set;
using std::size_t;

static void
writestr_bounded(
    int y, int x, const char *text, size_t length, short color = 0,
    bool bold = false) {
  if (bold != 0)
    attron(A_BOLD);
  if (color != 0)
    attron(COLOR_PAIR(color));
  if (x >= 0) {
    for (; x < COLS && *text; ++x, ++text)
      mvaddch(y, x, *text);
  } else if (static_cast<size_t>(-x + 2) < length) {
    mvaddch(y, 0, '<');
    mvaddch(y, 1, ' ');
    text += -x + 2;
    x = 2;
    for (; x < COLS && *text; ++x, ++text)
      mvaddch(y, x, *text);
  }
  if (x >= COLS && *text) {
    mvaddch(y, COLS - 2, ' ');
    mvaddch(y, COLS - 1, '>');
  }
  if (color != 0)
    attroff(COLOR_PAIR(color));
  if (bold != 0)
    attroff(A_BOLD);
}

static void
writestr_bounded(
    int y, int x, const string &text, short color = 0, bool bold = false) {
  writestr_bounded(y, x, text.c_str(), text.length(), color, bold);
}

static void
writech_bounded(int y, int x, unsigned ch, short color = 0) {
  if (y < 0 || x < 0 || y > LINES)
    return;
  if (color != 0)
    attron(COLOR_PAIR(color));
  mvaddch(y, x, ch);
  if (color != 0)
    attroff(COLOR_PAIR(color));
}

#define BASEY lines_ - winy_
#define BASEX f.depth * 2 - winx_

static void
update_max(int candidate, int &max) noexcept {
  if (candidate > max)
    max = candidate;
}

void
disp::repaint(settings &s, const vector<root> &roots) {
  clear();
  view_cols_ = 0;
  lines_     = 0;
  std::scoped_lock<std::mutex> l{s.roots_access_mutex};
  for (auto &&r : roots) {
    {
      // write the root title
      auto &&rpath = r.path().string();
      writestr_bounded(
          lines_++ - winy_, -winx_, rpath,
          fs::exists(r.path()) ? fs::is_directory(r.path()) ? PAIR_GREEN : 0
                               : PAIR_ERROR,
          fs::exists(r.path()) ? false : true);
      if (rpath.length() > static_cast<size_t>(view_cols_))
        view_cols_ = static_cast<int>(rpath.length());
    }
    // if the root is empty, continue
    if (r.files().empty()) {
      ++lines_;
      continue;
    }
    // set up border tracking for VLINE continuations
    set<int> border_xs;
    for (auto &&f : r.files()) {
      // store the branch character and update border_xs
      unsigned branch_ch;
      if (f.is_last_sibling) {
        branch_ch = ACS_LLCORNER;
        border_xs.erase(f.depth * 2);
      } else {
        branch_ch = ACS_LTEE;
        border_xs.insert(f.depth * 2);
      }

      // if the current line is outside of the viewing window, skip this file
      if (lines_ < winy_ || lines_ >= winy_ + LINES) {
        ++lines_;
        continue;
      }

      // write the borders
      for (auto &&x : border_xs)
        if (x != f.depth * 2)
          writech_bounded(BASEY, x - winx_, ACS_VLINE, PAIR_BLACK);
      writech_bounded(BASEY, BASEX, branch_ch, PAIR_BLACK);

      // write the file name
      auto &&fname = f.path.filename().string();
      writestr_bounded(
          BASEY, BASEX + 2, fname,
          f.is_symlink     ? PAIR_CYAN
          : f.is_directory ? PAIR_GREEN
                           : 0);

      // if the file is a symlink, write the target
      if (f.is_symlink) {
        try {
          auto &&path_name  = fs::read_symlink(f.path).string();
          fs::path sym_path = path_name;
          if (sym_path.is_relative()) {
            sym_path = f.path.parent_path().concat("/" + sym_path.string());
          } else {
            sym_path = fs::relative(sym_path);
          }
          if (!sym_path.empty()) {
            writestr_bounded(
                BASEY, BASEX + 2 + static_cast<int>(fname.length()) + 1, "->",
                2);
            bool exists = fs::exists(sym_path);
            writestr_bounded(
                BASEY, BASEX + 2 + static_cast<int>(fname.length()) + 4,
                path_name,
                exists ? fs::is_directory(sym_path) ? PAIR_GREEN : 0
                       : PAIR_ERROR,
                exists ? false : true);
            // update the view cols if this symlink name is above the previous
            // max
            update_max(
                BASEX + 2 + static_cast<int>(fname.length()) + 4 +
                    static_cast<int>(sym_path.string().length()),
                view_cols_);
          }
        } catch (...) {}
      }

      // update the view cols if this filename is above the previous max
      update_max(
          f.depth * 2 + 2 + static_cast<int>(fname.length()), view_cols_);

      ++lines_;
    }
    // add a line after each root
    ++lines_;
  }
  // subtract last extra line
  --lines_;
}

#define UPDATE_IGNORED(var)                               \
  s.var = !s.var;                                         \
  {                                                       \
    std::scoped_lock<std::mutex> l{s.roots_access_mutex}; \
    for (auto &&r : roots)                                \
      r.update(s);                                        \
  }                                                       \
  repaint(s, roots);                                      \
  if (winy_ > lines_)                                     \
    winy_ = lines_ - LINES;                               \
  if (winy_ < 0)                                          \
    winy_ = 0;                                            \
  repaint(s, roots)

#define ADJUST_WINX()            \
  if (winx_ >= view_cols_ - 3) { \
    winx_ = view_cols_ - 3;      \
    repaint(s, roots);           \
  }                              \
  do {                           \
  } while (0)

bool
disp::check_input(settings &s, vector<root> &roots) {
  timeout(1000);
  switch (getch()) {
  case 'h': {
    if (winx_ > 0) {
      --winx_;
      repaint(s, roots);
    }
  } break;
  case 'H': {
    if (winx_ - 10 > 0) {
      winx_ -= 10;
      repaint(s, roots);
    } else if (winx_ > 0) {
      winx_ = 0;
      repaint(s, roots);
    }
  } break;
  case 'j': {
    if (winy_ < lines_ - LINES) {
      ++winy_;
      repaint(s, roots);
      ADJUST_WINX();
    }
  } break;
  case 'J': {
    if (winy_ + 10 < lines_ - LINES) {
      winy_ += 10;
      repaint(s, roots);
      ADJUST_WINX();
    } else if (winy_ < lines_ - LINES) {
      winy_ = lines_ - LINES;
      repaint(s, roots);
      ADJUST_WINX();
    }
  } break;
  case 'k': {
    if (winy_ > 0) {
      --winy_;
      repaint(s, roots);
      ADJUST_WINX();
    }
  } break;
  case 'K': {
    if (winy_ - 10 > 0) {
      winy_ -= 10;
      repaint(s, roots);
      ADJUST_WINX();
    } else if (winy_ > 0) {
      winy_ = 0;
      repaint(s, roots);
      ADJUST_WINX();
    }
  } break;
  case 'l': {
    if (winx_ < view_cols_ - 3) {
      ++winx_;
      repaint(s, roots);
    }
  } break;
  case 'L': {
    if (winx_ + 10 < view_cols_ - 3) {
      winx_ += 10;
      repaint(s, roots);
    } else if (winx_ < view_cols_ - 3) {
      winx_ = view_cols_ - 3;
      repaint(s, roots);
    }
  } break;
#ifndef TREEE_NO_GIT
  case 'g': {
    UPDATE_IGNORED(read_gitignored);
  } break;
#endif
  case 'i': {
    UPDATE_IGNORED(read_patignored);
  } break;
  case '.': {
    UPDATE_IGNORED(read_hidden);
  } break;
  case '+': {
    bool stopped_early = false;
    {
      std::scoped_lock<std::mutex> l{s.roots_access_mutex};
      for (auto &&v : roots)
        if (v.stopped_early()) {
          stopped_early = true;
          break;
        }
    }
    if (s.max_depth == -1) {
      std::scoped_lock<std::mutex> l{s.roots_access_mutex};
      for (auto &&v : roots)
        if (v.max_depth() > s.max_depth)
          s.max_depth = v.max_depth();
    } else if (stopped_early) {
      ++s.max_depth;
      {
        std::scoped_lock<std::mutex> l{s.roots_access_mutex};
        for (auto &&v : roots)
          v.update(s);
        for (auto &&v : roots)
          v.update(s);
      }
      repaint(s, roots);
    }
  } break;
  case '-': {
    if (s.max_depth == -1) {
      std::scoped_lock<std::mutex> l{s.roots_access_mutex};
      for (auto &&v : roots)
        if (v.max_depth() > s.max_depth)
          s.max_depth = v.max_depth();
    }
    if (s.max_depth > 0) {
      --s.max_depth;
      {
        std::scoped_lock<std::mutex> l{s.roots_access_mutex};
        for (auto &&v : roots)
          v.update(s);
      }
      repaint(s, roots);
    }
    if (winy_ >= lines_)
      winy_ = lines_ > LINES ? lines_ - LINES : 0;
  } break;
  case 'q': return true;
  }
  return false;
}

} // namespace treee

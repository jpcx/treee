/* /////////////////////////////////////////////////////////////////////////////
//   _
//  | |_ _ __ ___  ___  ___   treee: an interactive file tree viewer
//  | __| '__/ _ \/ _ \/ _ \  Copyright (C) 2020-2023 Justin Collier
//  | |_| | |  __/  __/  __/
//   \__|_|  \___|\___|\___|  - - - - - - - - - - - - - - - - - -
//
//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
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

#include <iostream>
#include <thread>
#include <vector>

#include "treee/disp.h"
#include "treee/ncurses.h"
#include "treee/root.h"
#include "treee/settings.h"

static std::vector<treee::root>
parse_args(int argc, char **argv, treee::settings *s) {
  using namespace treee;
  std::vector<treee::root> roots{};
  bool expecting_ignore = false;
  bool expecting_level  = false;
  for (int i = 1; i < argc; ++i) {
    if (expecting_ignore) {
      s->ignore_pats.emplace_back(
          argv[i], std::regex::ECMAScript | std::regex::optimize);
      expecting_ignore = false;
    } else if (expecting_level) {
      try {
        s->max_depth = std::stoi(argv[i]) - 1;
        if (s->max_depth < 0) {
          std::cerr << "invalid level: must be greater than 0" << '\n';
          exit(1);
        }
      } catch (...) {
        std::cerr << "invalid level: must be greater than 0" << '\n';
        exit(1);
      }
      expecting_level = false;
    } else if (argv[i][0] == '-' && argv[i][1] == 'h' && argv[i][2] == '\0') {
      std::cerr << "usage: treee [-I PAT]... [-L LVL]\n";
      exit(0);
    } else if (argv[i][0] == '-' && argv[i][1] == 'I' && argv[i][2] == '\0') {
      expecting_ignore = true;
    } else if (argv[i][0] == '-' && argv[i][1] == 'L' && argv[i][2] == '\0') {
      expecting_level = true;
    } else {
      roots.emplace_back(fs::path{argv[i]});
    }
  }
  if (expecting_ignore || expecting_level) {
    std::cerr << "usage: treee [-I PAT]... [-L LVL]\n";
    exit(1);
  }
  if (roots.empty())
    roots.emplace_back(fs::path{"."});
  return roots;
}

int
main(int argc, char **argv) {
  using std::atomic;
  using std::vector;

  using namespace treee;

  settings s;
  auto roots = parse_args(argc, argv, &s);

  ncurses_init();

  disp d = disp();
  for (auto &&r : roots)
    r.update(s);

  std::atomic<bool> should_exit = false;

  std::thread update_thread{[&]() {
    while (!should_exit) {
      {
        std::scoped_lock<std::mutex> l{s.roots_access_mutex};
        for (auto &&r : roots)
          r.update(s);
      }
      for (int i = 0; i < 10 && !should_exit; ++i)
        std::this_thread::sleep_for(std::chrono::milliseconds{50});
    }
  }};
  std::thread paint_thread{[&]() {
    while (!should_exit) {
      d.repaint(s, roots);
      should_exit = d.check_input(s, roots);
    }
  }};

  update_thread.join();
  paint_thread.join();

  ncurses_cleanup();
}

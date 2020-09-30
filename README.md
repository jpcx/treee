# [treee](CHANGELOG.md)

```
 _
| |_ _ __ ___  ___  ___   treee: an interactive file tree viewer
| __| '__/ _ \/ _ \/ _ \  Copyright (C) 2020 Justin Collier
| |_| | |  __/  __/  __/
 \__|_|  \___|\___|\___|  - - - - - - - - - - - - - - - - - -
```

## About

treee is intended for use as an auxiliary window for software development.

- Use it to keep track of the relevant parts of your project structure
- Or use it as a `tree` replacement for general file browsing.

treee

- provides a live view of any number of directory trees
- optionally interfaces with libgit2 by default to skip gitignored paths for each tree
- allows for manual pattern match exclusions
- skips hidden files by default
- is multi-threaded and fast

![example](./.assets/tree.example.png)
![showhidden.example](./.assets/tree_showhidden.example.png)

## Requirements

- GNU/Linux
- >= C++17 (tested with Clang 10.0.0, Clang 11.0.0, GCC 10.2.1, and GCC 11.0.0)
- libgit2 [optional; compile with `cmake .. -DTREEE\_NO\_GIT=ON` to disable]
- ncurses
- pthread
- CMake >=3.0
- GNU Make [optional; Makefile calls CMake]

## Usage

```bash
git clone https://github.com/jpcx/treee
cd treee
make
sudo make install

# for this directory
treee

# for other directories:
treee ../another_dir

# for multiple directories:
treee . some_dir ../another_dir

# to exclude patterns (ECMAScript Regex)
treee -I thirdparty|assets
```

each directory argument provided

## Controls

`h`, `j`, `k`, `l` for motion

_[hold shift for fast motion]_

press `.` to toggle hidden file display
press `g` to toggle gitignored file display
press `i` to toggle pattern-ignored file display

press `q` to exit

## Contribution

Contribution is welcome! Please make a pull request.

## [License](LICENSE)

```
Copyright (C) 2020 Justin Collier

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the internalied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.
```

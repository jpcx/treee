#ifndef TREEE_FILE_H_INCLUDED
#define TREEE_FILE_H_INCLUDED
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

#include <stdint.h>

#include "str.h"
#include "tinydir.h"

/*.----------------------------------------------------------------------------,
 /                                    file                                   */

typedef struct file file;
struct file {
  str path;
  str name;
  str symlink_target;
  long level;
  int is_dir;
  int is_lnk;
  int is_valid_lnk;
  file *parent;
  file *first_child;
  file *next_sibling;
};

file
file_ctor(
    file *parent, const char *filename, const int is_dir, const int is_lnk);

file *
file_new(
    file *parent, const char *filename, const int is_dir, const int is_lnk);

void
file_insert(file *parent, file *newchild);

void
file_destruct(file *f);

void
file_attach(file *parent, tinydir_dir *dir);

int
file_has_siblings(file *node);

int
file_is_last_sibling(file *node);

#endif

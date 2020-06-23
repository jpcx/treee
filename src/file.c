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

#include <unistd.h>

/*.----------------------------------------------------------------------------,
 /                                  helpers                                  */

static int
alphacmp(const char *a, const char *b) {
  int i = 0;
  for (; a[i] != '\0' && b[i] != '\0'; ++i) {
    /* if (a[i] > b[i]) */
    /*   return 1; */
    /* else if (a[i] < b[i]) */
    /*   return -1; */
    if (a[i] != b[i])
      return -1 * (a[i] < b[i]) + (a[i] > b[i]);
  }
  /*      if (a[i] == b[i]) return  0; */
  /* else if (a[i] == '\0') return -1; */
  /* else                   return  1; */
  return (a[i] != b[i]) * (-2 * (a[i] == '\0')) + 1;
}

/*.----------------------------------------------------------------------------,
 /                                    file                                   */

file
file_ctor(
    file *parent, const char *filename, const int is_dir, const int is_lnk) {
  str path;
  long level;
  if (parent == NULL) {
    path  = str_new(filename);
    level = 0;
  } else {
    path = str_alloc(str_len(parent->path) + strlen(filename) + 1);
    str_append_(&path, parent->path);
    str_append(&path, "/");
    str_append(&path, filename);
    level = parent->level + 1;
  }

  char buf[_TINYDIR_PATH_MAX];
  size_t len = readlink(path, buf, sizeof(buf) - 1);
  if (len != -1) {
    buf[len] = '\0';
  }
  int is_valid_lnk = 0;
  if (len != -1 && access(buf, F_OK) != -1) {
    is_valid_lnk = 1;
  }

  return (file){
      .path           = path,
      .name           = str_new(filename),
      .symlink_target = len != -1 ? str_new(buf) : str_new(""),
      .level          = level,
      .is_dir         = is_dir,
      .is_lnk         = len != -1,
      .is_valid_lnk   = is_valid_lnk,
      .parent         = parent,
      .first_child    = NULL,
      .next_sibling   = NULL,
  };
}

file *
file_new(
    file *parent, const char *filename, const int is_dir, const int is_lnk) {
  file *o  = malloc(sizeof(file));
  file obj = file_ctor(parent, filename, is_dir, is_lnk);
  memcpy(o, &obj, sizeof(obj));
  return o;
}

void
file_insert(file *parent, file *newchild) {
  file *cur        = parent->first_child;
  newchild->parent = parent;

  if (cur == NULL) {
    parent->first_child = newchild;
    return;
  }

  // if child name < first_child name, make it the first child and return
  if (alphacmp(newchild->name, cur->name) < 0) {
    newchild->next_sibling = cur;
    parent->first_child    = newchild;
    return;
  }

  // loop through all siblings until child is < the next sibling (or next
  // sibling is NULL), insert between cur and next and return
  for (; cur->next_sibling != NULL; cur = cur->next_sibling) {
    if (alphacmp(newchild->name, cur->next_sibling->name) < 0) {
      newchild->next_sibling = cur->next_sibling;
      cur->next_sibling      = newchild;
      return;
    }
  }

  // next_sibling is NULL; set it to child
  cur->next_sibling = newchild;
}

// destroys all children and their siblings recursively
void
file_destruct(file *f) {
  for (file *cur = f->first_child; cur != NULL; cur = cur->next_sibling) {
    file_destruct(cur);
  }
  str_free(&f->path);
  str_free(&f->name);
  str_free(&f->symlink_target);
  free(f);
}

void
file_attach(file *parent, tinydir_dir *dir) {
  while (dir->has_next) {
    tinydir_file tdf;
    tinydir_readfile(dir, &tdf);

    if (strcmp(tdf.name, ".") == 0 || strcmp(tdf.name, "..") == 0) {
      tinydir_next(dir);
      continue; // skip ./ and ../
    }

    file *next =
        file_new(parent, tdf.name, tdf.is_dir, S_ISREG(tdf._s.st_mode));
    file_insert(parent, next);

    if (tdf.is_dir) {
      tinydir_dir tdnext;
      tinydir_open(&tdnext, next->path);
      file_attach(next, &tdnext);
      tinydir_close(&tdnext);
    }

    tinydir_next(dir);
  }
}

int
file_has_siblings(file *node) {
  file *parent = node->parent;
  if (parent == NULL)
    return 1;
  return node != parent->first_child || node->next_sibling != NULL;
}

int
file_is_last_sibling(file *node) {
  file *parent = node->parent;
  if (parent == NULL)
    return 1;
  file *lst;
  for (lst = parent->first_child; lst->next_sibling != NULL;
       lst = lst->next_sibling)
    ;
  return lst == node;
}

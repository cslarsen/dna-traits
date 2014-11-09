/*
 * Copyright (C) 2014 Christian Stigen Larsen
 * Distributed under the GPL v3 or later. See COPYING.
 */

#ifndef DNA_FILE_H
#define DNA_FILE_H

class File {
  int fd;
public:
  File(const char* filename, const int flags);
  ~File();

  inline operator int() const {
    return fd;
  }
};

#endif

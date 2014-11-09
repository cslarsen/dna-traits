/*
 * Copyright (C) 2014 Christian Stigen Larsen
 * Distributed under the GPL v3 or later. See COPYING.
 */

#ifndef DNA_FILEPTR_H
#define DNA_FILEPTR_H

#include <stdio.h>

class FilePtr {
  FILE *f;
public:
  FilePtr(const char* file, const char* mode);
  ~FilePtr();

  inline operator FILE*() const {
    return f;
  }

  FILE* ptr() const {
    return f;
  }
};

#endif

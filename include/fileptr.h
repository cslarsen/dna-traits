/*
 * Copyright (C) 2014 Christian Stigen Larsen
 * Distributed under the GPL v3 or later. See COPYING.
 */

#ifndef DNA_FILEPTR_H
#define DNA_FILEPTR_H

#include <string>
#include <stdio.h>

class FilePtr {
  FILE *f;
public:
  FilePtr(const std::string& file, const char* mode):
    f(fopen(file.c_str(), mode))
  {
    if ( !f )
      throw std::runtime_error("Could not open file: " + file);
  }

  ~FilePtr() {
    fclose(f);
  }

  inline operator FILE*() const {
    return f;
  }

  FILE* ptr() const {
    return f;
  }
};

#endif

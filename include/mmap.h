/*
 * Copyright (C) 2014 Christian Stigen Larsen
 * Distributed under the GPL v3 or later. See COPYING.
 */

#include <exception>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/types.h>

class MMap {
  size_t l;
  void *p;
public:
  MMap(void *address,
       size_t length,
       int protection_level,
       int flags,
       int file_descriptor,
       off_t offset)
  : l(length),
    p(mmap(address,
           length,
           protection_level,
           flags,
           file_descriptor,
           offset))
  {
    if ( p == reinterpret_cast<caddr_t>(-1) )
      throw std::runtime_error("mmap error");
  }

  ~MMap() {
    munmap(p, l);
  }

  inline void* ptr() const {
    return p;
  }
};

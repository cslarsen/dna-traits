/*
 * Quite Fast reading of 23andme files (only RSID->Genotype map) using
 * memory maps, fast C-style parsing, two passes (vector first, hash map
 * insert afterwards) and C++11 containers.
 *
 * This is just an experiment on how fast I can parse those files.
 *
 * On my i7 with a slow SSD, I parse an entire 24Mb 23andmefile in 0.5 secs,
 * using only 2 context switches (best case numbers).  That works out to a
 * parsing speed of about 48Mb/s, with disk max being 175Mb/s, so an
 * overhead of 75% (i.e., 24% of theoretical maximum read).
 *
 * Copyright (C) 2014 Christian Stigen Larsen
 * Distributed under the GPL v3 or later.
 */

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <string>
#include <vector>
#include <unordered_map>
#include <inttypes.h>

class File {
  int fd;
public:
  File(const std::string& file, const int flags):
    fd(open(file.c_str(), flags))
  {
    if ( fd < 0 )
      std::runtime_error("Could not open file: " + file);
  }

  ~File() {
    close(fd);
  }

  inline operator int() const {
    return fd;
  }
};

struct String {
  const char* ptr;
  size_t len;

  void stop(const char* offset)
  {
    len = offset - ptr;
  }

  std::string str() const
  {
    return std::string(ptr, ptr+len);
  }
};

typedef uint32_t RSID;

typedef std::pair<char, char> Genotype;

struct SNP {
  RSID rsid;
  Genotype genotype;
};

static inline const char*& skipfield(const char*& s)
{
  while ( (*s != '\t') &&
          (*s != '\r') &&
          (*s != '\n') ) ++s;
  ++s;
  return s;
}

static String readfield(const char*& s)
{
  String str;
  str.ptr = s;
  str.stop(skipfield(s)-1);
  return str;
}

static off_t filesize(const int file_descriptor)
{
  struct stat stat;

  if ( fstat(file_descriptor, &stat) < 0 )
    throw std::runtime_error("Could not stat file");

  return stat.st_size;
}

class MMap {
  size_t _len;
  void *_ptr;
public:
  MMap(void *addr, size_t len, int prot, int flags, int fd, off_t offset):
    _len(len),
    _ptr(mmap(addr, len, prot, flags, fd, offset))
  {
    if ( _ptr == reinterpret_cast<caddr_t>(-1) )
      throw std::runtime_error("mmap error");
  }

  ~MMap() {
    munmap(_ptr, _len);
  }

  inline void* ptr() const {
    return _ptr;
  }
};

int parse(const char* file, std::unordered_map<RSID, Genotype>& map)
{
  File fd(file, O_RDONLY);
  MMap fdmap(0, filesize(fd), PROT_READ, MAP_PRIVATE, fd, 0);
  const char *s = static_cast<const char*>(fdmap.ptr());

  // skip comments in header
  while ( *s == '#' ) {
    while ( *s != '\n' ) ++s;
    ++s;
  }

  // parse SNPs
  for ( ; *s; ++s ) {
    // rsid: skip internal ids, etc
    auto id = readfield(s);
    RSID rsid = id.ptr[0]=='r'? atoi(id.ptr+2) : 0;

    // skip chromosome, but note if mitochondrial dna
    auto MT = (*s=='M');
    skipfield(s);

    // skip offset
    skipfield(s);

    const String geno = readfield(s);
    Genotype genotype(geno.ptr[0],
                      geno.len>1? geno.ptr[1] : ' ');

    // skip internal ids, mitochondrial dna, etc.
    if ( rsid > 0 && !MT ) {
      map.insert({rsid, genotype});
    }
  }

  return 0;
}

int main(int argc, char** argv)
{
  std::unordered_map<RSID, Genotype> map(1000000);
  parse(argv[1], map);
  printf("DONE, hash table has %lu entries\n", map.size());

  RSID id = (*map.begin()).first;
  Genotype gt = (*map.begin()).second;

  printf("Lowest-numbered SNP is rs%u with genotype %c%c\n",
    id, gt.first, gt.second);
}

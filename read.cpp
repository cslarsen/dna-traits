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

#include "file.h"
#include "mmap.h"
#include "filesize.h"

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
#include <string>
#include <unordered_map>

typedef uint32_t RSID;
typedef std::pair<char, char> Genotype;
typedef std::unordered_map<RSID, Genotype> SNPMap;

struct String {
  const char* ptr;
  off_t len;
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
  return String {s, skipfield(s)-s-1};
}

void parse(const char* file, SNPMap& map)
{
  File fd(file, O_RDONLY);
  MMap fdmap(0, filesize(fd), PROT_READ, MAP_PRIVATE, fd, 0);
  auto s = static_cast<const char*>(fdmap.ptr());

  // Skip comments in header
  while ( *s == '#' )
    while ( *s++ != '\n' );

  // Parse SNPs
  for ( ; *s; ++s ) {
    // Parse RSID
    String str = readfield(s);
    RSID rsid(str.ptr[0]=='r'? atoi(str.ptr+2) : 0);

    // Mitochonrdial chromosome?
    const bool MT = (*s=='M');
    skipfield(s); // skip chromosome

    // Skip offset
    skipfield(s);

    // Parse genotype
    str = readfield(s);
    Genotype genotype(str.ptr[0],
                      str.len>1? str.ptr[1] : ' ');

    // Skip internal IDs and mitochondria
    if ( rsid>0 && !MT )
      map.insert({rsid, genotype});
  }
}

void print_rsid(const RSID& id, const SNPMap& map)
{
  auto genotype = map.at(id);
  printf("rs%u has genotype %c%c\n", id, genotype.first, genotype.second);
}

int main(int argc, char** argv)
{
  if ( argc < 1 )
    return 1;

  auto file = argv[1];

  printf("Reading %s ... ", file);

  SNPMap map(1000000);
  parse(file, map);

  printf("done\n");
  printf("Read %lu SNPs\n", map.size());

  printf("Lowest-numbered SNP: ");

  if ( !map.empty() )
    print_rsid(map.begin()->first, map);
}

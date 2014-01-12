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

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h> /* mmap() is defined in this header */
#include <fcntl.h>
#include <string>
#include <vector>
#include <unordered_map>
#include <inttypes.h>

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

// typedef char Genotype[2]; does not work with std::map
struct Genotype {
  char a, b;
};

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

int parse(const char* file, std::vector<SNP>& snps)
{
  int fd = open(file, O_RDONLY);
  if ( fd < 0 ) {
    fprintf(stderr, "Could not open file: %s\n", file);
    return 1;
  }

  struct stat statbuf;
  if ( fstat(fd, &statbuf) < 0 ) {
    fprintf(stderr, "Could not stat file: %s\n", file);
    return 1;
  }

  const char *s = static_cast<const char*>(
                    mmap(0, statbuf.st_size,
                         PROT_READ, MAP_PRIVATE, fd, 0));
  if ( s == reinterpret_cast<caddr_t>(-1) ) {
    fprintf(stderr, "MMAP error\n");
    return 1;
  }

  // skip comments in header
  while ( *s == '#' ) {
    while ( *s != '\n' ) ++s;
    ++s;
  }

  // parse SNPs
  for ( ; *s; ++s ) {
    SNP snp;

    // rsid: skip internal ids, etc
    String id = readfield(s);
    snp.rsid = id.ptr[0]=='r'? atoi(id.ptr+2) : 0;

    // skip chromosome, but note if mitochondrial dna
    bool MT = (*s=='M');
    skipfield(s);

    // skip offset
    skipfield(s);

    String genotype = readfield(s);
    snp.genotype.a = genotype.ptr[0];
    snp.genotype.b = genotype.len>1? genotype.ptr[1] : ' ';

    // skip internal ids, mitochondrial dna, etc.
    if ( snp.rsid > 0 && !MT )
      snps.push_back(snp);
  }

  return 0;
}

int main(int argc, char** argv)
{
  std::vector<SNP> snps;
  parse(argv[1], snps);
  printf("READ %lu SNPs\n", snps.size());
  printf("TOP rs%u geno: %c%c\n",
      snps[0].rsid,
      snps[0].genotype.a,
      snps[0].genotype.b);
  printf("BOT rs%u geno: %c%c\n",
      snps[snps.size()-1].rsid,
      snps[snps.size()-1].genotype.a,
      snps[snps.size()-1].genotype.b);

  // put into map
  std::unordered_map<RSID, Genotype> map(snps.size());
  auto collisions = 0;
  for ( auto snp : snps ) {
    if ( map.find(snp.rsid) != map.end() )
      ++collisions;
    map.insert({snp.rsid, snp.genotype});
  }

  printf("DONE, hash table has %lu entries\n", map.size());
  printf("%d collisions\n", collisions);
}

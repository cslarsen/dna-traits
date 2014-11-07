/*
 * Copyright (C) 2014 Christian Stigen Larsen
 * Distributed under the GPL v3 or later. See COPYING.
 */

#include "dna.h"
#include "file.h"
#include "filesize.h"
#include "mmap.h"

static Nucleotide CharToNucleotide[256] = {NONE};

static inline void skip_comments(const char*& s)
{
  while ( *s == '#' )
    while ( *s++ != '\n' )
      ; // loop
}

static inline const char*& skipwhite(const char*& s)
{
  while ( (*s == '\t') ||
          (*s == '\r') ||
          (*s == '\n') ) ++s;
  return s;
}

static inline const char*& skiptab(const char*& s)
{
  while ( (*s != '\t') &&
          (*s != '\r') &&
          (*s != '\n') ) ++s;
  return s;
}

static inline const char*& skipnext(const char*& s)
{
  skiptab(s);
  skipwhite(s);
  return s;
}

static inline uint32_t parse_uint32(const char*& s)
{
  uint32_t n = 0;

  while ( isdigit(*s) )
    n = n*10 + *s++ - '0';

  return n;
}

static inline Nucleotide parse_nucleotide(const char*& s)
{
  return CharToNucleotide[static_cast<const std::size_t>(*s++)];
}

static inline Genotype parse_genotype(const char*& s)
{
  return Genotype(parse_nucleotide(s),
                  parse_nucleotide(s));
}

static inline void skipline(const char*& s)
{
  while ( *s != '\n' ) ++s;
}

/**
 * Reads a 23andMe-formatted genome file.  It currently uses reference human
 * assembly build 37 (annotation release 104).
 */
void parse_file(const std::string& name, DNA& dna)
{
  CharToNucleotide[static_cast<unsigned>('A')] = A;
  CharToNucleotide[static_cast<unsigned>('G')] = G;
  CharToNucleotide[static_cast<unsigned>('C')] = C;
  CharToNucleotide[static_cast<unsigned>('T')] = T;
  CharToNucleotide[static_cast<unsigned>('D')] = D;
  CharToNucleotide[static_cast<unsigned>('I')] = I;

  File fd(name, O_RDONLY);
  MMap fmap(0, filesize(fd), PROT_READ, MAP_PRIVATE, fd, 0);
  auto s = static_cast<const char*>(fmap.ptr());

  dna.ychromo = false;
  skip_comments(s);

  for ( ; *s; ++s ) {
    // Skip anything other than an RSID (internal IDs, etc.)
    if ( *s != 'r' ) {
      skipline(s);
      continue;
    }

    RSID rsid(parse_uint32(s+=2)); // rs[0-9]+
    dna.ychromo |= (*skipwhite(s)=='Y'); // has Y chromosome?

    dna.snp.insert({rsid, parse_genotype(skipnext(skipnext(s)))});
  }
}

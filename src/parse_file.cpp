/*
 * Copyright (C) 2014 Christian Stigen Larsen
 * Distributed under the GPL v3 or later. See COPYING.
 */

#include "dna.h"
#include "file.h"
#include "filesize.h"
#include "mmap.h"

struct String {
  const char* ptr;
  off_t len;
};

static void skip_comments(const char*& s)
{
  while ( *s == '#' )
    while ( *s++ != '\n' )
      ; // loop
}

static inline const char*& skipfield(const char*& s)
{
  while ( (*s != '\t') &&
          (*s != '\r') &&
          (*s != '\n') ) ++s;
  return ++s;
}

static inline String readfield(const char*& s)
{
  return String {s, skipfield(s)-s-1};
}

static Nucleotide nuclmap[256] = {NONE};

static inline Nucleotide to_nucleotide(const char ch)
{
  return nuclmap[static_cast<const std::size_t>(ch)];
}

static Chromosome parse_chromo(const char* s)
{
  switch (*s) {
    case '0': case '1': case '2': case '3': case '4': case '5':
    case '6': case '7': case '8': case '9':
      return static_cast<Chromosome>(atoi(s));
    case 'M': return CHR_MT;
    case 'X': return CHR_X;
    case 'Y': return CHR_Y;
    default: return NO_CHR;
  }
}

static inline void skipline(const char*& s)
{
  while ( *s != '\n' ) ++s;
}

static inline void skiptab(const char*& s)
{
  while ( *s++ != '\t' );
}

/**
 * Reads a 23andMe-formatted genome file.  It currently uses reference human
 * assembly build 37 (annotation release 104).
 */
void parse_file(const std::string& name, DNA& dna)
{
  nuclmap[static_cast<unsigned>('A')] = A;
  nuclmap[static_cast<unsigned>('G')] = G;
  nuclmap[static_cast<unsigned>('C')] = C;
  nuclmap[static_cast<unsigned>('T')] = T;
  nuclmap[static_cast<unsigned>('D')] = D;
  nuclmap[static_cast<unsigned>('I')] = I;

  File fd(name, O_RDONLY);
  MMap fmap(0, filesize(fd), PROT_READ, MAP_PRIVATE, fd, 0);
  auto s = static_cast<const char*>(fmap.ptr());

  dna.ychromo = false;
  skip_comments(s);

  for ( String str; *s; ++s ) {
    // Skip anything other than an RSID (internal IDs, etc.)
    if ( *s != 'r' ) {
      skipline(s);
      continue;
    }

    RSID rsid(atoi(readfield(s).ptr+2)); // "rs<number>"

    // Store if genome has a Y-chromosome
    dna.ychromo |= (*s=='Y');

    // Chromosome [1-22|X|Y|MT]
    Chromosome chr(parse_chromo(s));

    // Position
    skipfield(s);
    Position pos(atoi(readfield(s).ptr));

    // Genotype
    Genotype gen(to_nucleotide(s[0]),
                 to_nucleotide(s[1]));
    dna.snp.insert({rsid, SNP(gen, chr, pos)});
  }
}


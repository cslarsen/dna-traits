/*
 * Copyright (C) 2014 Christian Stigen Larsen
 * Distributed under the GPL v3 or later. See COPYING.
 */

#include "dnatraits.hpp"
#include "file.hpp"
#include "filesize.hpp"
#include "mmap.hpp"

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

static inline Chromosome parse_chromo(const char*& s)
{
  if ( isdigit(*s) )
      return static_cast<Chromosome>(parse_uint32(s));

  switch ( *s++ ) {
    case 'M': ++s; return CHR_MT;
    case 'X': return CHR_X;
    case 'Y': return CHR_Y;
    default: return NO_CHR;
  }
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
void parse_file(const std::string& name, Genome& genome)
{
  CharToNucleotide[static_cast<unsigned>('A')] = A;
  CharToNucleotide[static_cast<unsigned>('G')] = G;
  CharToNucleotide[static_cast<unsigned>('C')] = C;
  CharToNucleotide[static_cast<unsigned>('T')] = T;
  CharToNucleotide[static_cast<unsigned>('D')] = D;
  CharToNucleotide[static_cast<unsigned>('I')] = I;

  File fd(name.c_str(), O_RDONLY);
  MMap fmap(0, filesize(fd), PROT_READ, MAP_PRIVATE, fd, 0);
  auto s = static_cast<const char*>(fmap.ptr());

  genome.y_chromosome = false;
  skip_comments(s);

  for ( ; *s; ++s ) {
    // Skip anything other than an RSID (internal IDs, etc.)
    if ( *s != 'r' ) {
      skipline(s);
      continue;
    }

    RSID rsid(parse_uint32(s+=2)); // rs[0-9]+
    genome.first = rsid < genome.first? rsid : genome.first;
    genome.last = rsid > genome.last? rsid : genome.last;
    genome.y_chromosome |= (*skipwhite(s)=='Y'); // is Y chromosome?

    genome.insert(rsid, SNP(parse_chromo(s),
                            parse_uint32(skipwhite(s)),
                            parse_genotype(skipwhite(s))));
  }
}

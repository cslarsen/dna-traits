/*
 * Copyright (C) 2014 Christian Stigen Larsen
 * Distributed under the GPL v3 or later. See COPYING.
 */

#include "dna.h"
#include "file.h"
#include "filesize.h"
#include "mmap.h"

static Nucleotide NucleotideMap[256] = {NONE};

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
  return NucleotideMap[static_cast<const std::size_t>(*s++)];
}

static inline Chromosome parse_chromo(const char*& s)
{
  if ( isdigit(*s) )
      return static_cast<Chromosome>(parse_uint32(s));

  switch ( *s++ ) {
    case 'M': return CHR_MT;
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
void parse_file(const std::string& name, DNA& dna)
{
  NucleotideMap[static_cast<unsigned>('A')] = A;
  NucleotideMap[static_cast<unsigned>('G')] = G;
  NucleotideMap[static_cast<unsigned>('C')] = C;
  NucleotideMap[static_cast<unsigned>('T')] = T;
  NucleotideMap[static_cast<unsigned>('D')] = D;
  NucleotideMap[static_cast<unsigned>('I')] = I;

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

    // "rs[0-9]+"
    RSID rsid(parse_uint32(s+=2));
    skipwhite(s);

    // Store if genome has a Y-chromosome
    dna.ychromo |= (*s=='Y');

    SNP snp;

    // Chromosome [1-22|X|Y|MT]
    snp.chromosome = parse_chromo(s);
    skipwhite(s);

    // Position
    snp.position = parse_uint32(s);
    skipwhite(s);

    // Genotype
    snp.genotype = parse_genotype(s);
    dna.snp.insert({rsid, snp});
  }
}

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

static const char*& skipfield(const char*& s)
{
  while ( (*s != '\t') &&
          (*s != '\r') &&
          (*s != '\n') ) ++s;
  ++s;
  return s;
}

static inline String readfield(const char*& s)
{
  return String {s, skipfield(s)-s-1};
}

Nucleotide to_nucleotide(const char ch)
{
  switch ( ch ) {
    case 'A': return A;
    case 'G': return G;
    case 'C': return C;
    case 'T': return T;
    case 'D': return D;
    case 'I': return I;
    case '\r':
    case '-': return NONE;
    default: {
      std::ostringstream s;
      s << "Unknown nucleotide character w/value "
        << static_cast<int>(ch);
      throw std::runtime_error(s.str().c_str());
    } break;
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

    str = readfield(s);
    RSID rsid(atoi(str.ptr+2)); // "rs<number>"

    // Skip mitochondrial DNA
    if ( *s == 'M' ) { // "MT"
      skipline(s);
      continue;
    }

    // Store if genome has a Y-chromosome
    dna.ychromo |= (*s=='Y');

    skiptab(s); // skip chromosome [1-22|X|Y|MT]
    skiptab(s); // skip position

    str = readfield(s); // genotype
    Genotype genotype(to_nucleotide(str.ptr[0]),
                      to_nucleotide(str.ptr[1]));

    dna.snp.insert({rsid, genotype});
  }
}

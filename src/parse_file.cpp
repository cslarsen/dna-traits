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

static String readfield(const char*& s)
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
     default: return NONE;
  }
}

/**
 * Reads a 23andme-formatted genome file.  It currently uses reference human
 * assembly build 37 (annotation release 104).
 */
void parse_file(const std::string& name, DNA& dna)
{
  File fd(name, O_RDONLY);
  MMap fmap(0, filesize(fd), PROT_READ, MAP_PRIVATE, fd, 0);
  auto s = static_cast<const char*>(fmap.ptr());

  skip_comments(s);

  for ( ; *s; ++s ) {
    // Parse RSID
    String str = readfield(s);
    RSID rsid(str.ptr[0]=='r'? atoi(str.ptr+2) : 0);

    // Mitochondrial DNA?
    const bool MT = (*s == 'M');

    if ( *s == 'Y' )
      dna.ychromo = true;

    skipfield(s); // skip chromosome
    skipfield(s); // skip offset

    // Parse genotype
    str = readfield(s);
    Genotype genotype(to_nucleotide(str.ptr[0]),
                      to_nucleotide(str.ptr[1]));

    // Only add RSIDs; skip internal IDs and MT
    if ( rsid && !MT )
      dna.snp.insert({rsid, genotype});
  }
}

#include <fstream>
#include <string>
#include <sstream>
#include "dna.h"

/**
 * Reads a 23andme-formatted genome file.  It currently uses reference human
 * assembly build 37 (annotation release 104).
 */
DNA parse_file(const std::string& name)
{
  using namespace std;

  DNA dna;
  ifstream f(name);

  if ( !f.is_open() )
    throw runtime_error("Could not open " + name);

  string token;
  while ( f >> token ) {

    // if comment, skip rest of line
    if ( token[0] == '#' ) {
      getline(f, token);
      continue;
    }

    SNP snp;
    stringstream(token) >> snp.rsid;
    f >> snp.chromosome;
    f >> snp.position;
    f >> snp.genotype;
    dna.insert({snp.rsid, snp});
  }

  return dna;
}

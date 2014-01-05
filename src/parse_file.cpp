#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "dna.h"

/**
 * Reads a 23andme-formatted genome file.  It currently uses reference human
 * assembly build 37 (annotation release 104).
 */
void parse_file(const std::string& name, DNA& dna)
{
  using namespace std;

  ifstream f(name);

  if ( !f.is_open() )
    throw runtime_error("Could not open " + name);

  string id, foo;
  while ( f >> id ) {

    // if comment, skip rest of line
    if ( id[0] == '#' ) {
      getline(f, id);
      continue;
    }

    SNP snp;
    std::stringstream(id) >> snp.id;
    f >> snp.chromosome;
    f >> snp.position;
    f >> snp.genotype;

    dna.snps.insert({id, snp});
  }
}

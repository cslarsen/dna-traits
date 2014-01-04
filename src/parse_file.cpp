#include <iostream>
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

  string id;
  while ( f >> id ) {

    // if comment, skip rest of line
    if ( id[0] == '#' ) {
      getline(f, id);
      continue;
    }

    SNP snp;
    snp.id = id;
    f >> snp.chromosome;
    f >> snp.position;
    f >> snp.genotype;

    dna.insert({id, snp});
  }

  return dna;
}

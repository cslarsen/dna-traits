#include <fstream>
#include <string>
#include <sstream>
#include <stdio.h>
#include "dna.h"

using namespace std;

DNA parse_file(const string& name)
{
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
  cout << endl;

  return dna;
}

int main(int argc, char** argv)
{
  try {
    for ( int n=1; n<argc; ++n ) {
      auto file = argv[n];
      cout << "Reading " << file << endl;
      auto dna = parse_file(file);
      cout << "Done" << endl;
    }
    return 0;
  } catch ( const exception& e) {
    cout << e.what() << endl;
    return 1;
  }
}

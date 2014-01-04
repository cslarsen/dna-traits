#include <fstream>
#include <iostream>
#include <unordered_map>
#include <string>
#include <sstream>
#include <stdio.h>
#include <inttypes.h>

using namespace std;

enum Nucleotide {
  MISSING, A, G, C, T
};

typedef pair<Nucleotide, Nucleotide> BasePair;
typedef uint32_t RSID;
typedef uint32_t Position;
typedef uint8_t Chromosome;

struct SNP {
  RSID rsid;
  Position position;
  BasePair genotype;
  Chromosome chromosome;
};

typedef unordered_map<RSID, SNP> DNA;

istream& operator>>(istream& i, Nucleotide& n)
{
  char ch;
  i >> ch;

  switch ( ch ) {
    case 'A': n = A; break;
    case 'G': n = G; break;
    case 'C': n = C; break;
    case 'T': n = T; break;
    default: n = MISSING; break;
  }

  return i;
}

istream& operator>>(istream& i, BasePair& bp)
{
  i >> bp.first;
  i >> bp.second;
  return i;
}


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

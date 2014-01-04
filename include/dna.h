#include <iostream>
#include <unordered_map>
#include <inttypes.h>

enum Nucleotide {
  MISSING, A, G, C, T
};

typedef std::pair<Nucleotide, Nucleotide> BasePair;
typedef uint32_t RSID;
typedef uint32_t Position;
typedef uint8_t Chromosome;

struct SNP {
  RSID rsid;
  Position position;
  BasePair genotype;
  Chromosome chromosome;
};

typedef std::unordered_map<RSID, SNP> DNA;

std::istream& operator>>(std::istream& i, Nucleotide& n);
std::istream& operator>>(std::istream& i, BasePair& bp);

#include <string>
#include <iostream>
#include <unordered_map>
#include <inttypes.h>

enum Nucleotide {
  NONE, A, G, C, T
};

/*
 * TODO:
 *
 * Improve data structure:
 *
 *   - RSID/internal id should only occupy 32-bits
 *   - Chromosome should only occupy 6-8 bits (numbers 1-22, X, Y)
 *
 */

typedef std::pair<Nucleotide, Nucleotide> BasePair; // [AGCT-]
typedef std::string ID; // (rs|i)[0-9]+
typedef uint32_t Position; // [0-9]+
typedef std::string Chromosome; // [0-9]+|X|Y

struct SNP {
  ID id;
  BasePair genotype;
  Chromosome chromosome;
  Position position;
};

typedef std::unordered_map<ID, SNP> DNA;

std::istream& operator>>(std::istream&, Nucleotide&);
std::istream& operator>>(std::istream&, BasePair&);
std::ostream& operator<<(std::ostream&, Nucleotide&);
std::ostream& operator<<(std::ostream&, BasePair&);
std::ostream& operator<<(std::ostream&, SNP&);

DNA parse_file(const std::string& filename);

#include <iostream>
#include <sstream>
#include <string>
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

typedef std::pair<Nucleotide, Nucleotide> Genotype; // [AGCT-]

// Some shorthand constants
const Genotype AA (A, A);
const Genotype AT (A, T);
const Genotype AC (A, C);
const Genotype AG (A, G);
const Genotype TA (T, A);
const Genotype TT (T, T);
const Genotype TC (T, C);
const Genotype TG (T, G);
const Genotype CA (C, A);
const Genotype CT (C, T);
const Genotype CC (C, C);
const Genotype CG (C, G);
const Genotype GA (G, A);
const Genotype GT (G, T);
const Genotype GC (G, C);
const Genotype GG (G, G);

#pragma pack(1)
struct ID {
  enum Type {
    RSID,
    INTERNAL_ID
  } type : 1;

  uint32_t index;

  ID(const char* s) : ID(std::string(s))
  {
  }

  ID(const std::string& s)
  {
    std::stringstream(s) >> *this;
  }

  ID() : type(RSID), index(0)
  {
  }
};

template<>
struct std::hash<ID> {
  std::size_t operator()(const ID& id) const
  {
    return std::hash<uint32_t>()(id.index)
         ^ ((id.type & 0x3) << 29);
  }
};

typedef uint32_t Position; // [0-9]+
typedef std::string Chromosome; // [0-9]+|X|Y|MT

struct SNP {
  ID id;
  Genotype genotype;
  Chromosome chromosome;
  Position position;
};

struct DNA {
  std::unordered_map<ID, SNP> snps;

  const SNP operator[](const ID& id) const
  {
    return snps.at(id);
  }
};

bool operator==(const ID&, const ID&);
std::istream& operator>>(std::istream&, ID&);
std::istream& operator>>(std::istream&, Genotype&);
std::istream& operator>>(std::istream&, Nucleotide&);
std::ostream& operator<<(std::ostream&, const ID& id);
std::ostream& operator<<(std::ostream&, const Genotype&);
std::ostream& operator<<(std::ostream&, const Nucleotide&);
std::ostream& operator<<(std::ostream&, const SNP&);

void parse_file(const std::string& filename, DNA&);
void summary(DNA&);

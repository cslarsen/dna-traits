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

typedef std::pair<Nucleotide, Nucleotide> BasePair; // [AGCT-]

#pragma pack(1)
struct ID {
  enum Type {
    RSID,
    INTERNAL_ID
  } type : 1;

  uint32_t index;

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
  BasePair genotype;
  Chromosome chromosome;
  Position position;
};

struct DNA {
  std::unordered_map<ID, SNP> snps;

  const SNP operator[](const ID& id) const
  {
    return snps.at(id);
  }

  const std::size_t size() const
  {
    return snps.size();
  }
};

bool operator==(const ID&, const ID&);
std::istream& operator>>(std::istream&, ID&);
std::istream& operator>>(std::istream&, BasePair&);
std::istream& operator>>(std::istream&, Nucleotide&);
std::ostream& operator<<(std::ostream&, const ID& id);
std::ostream& operator<<(std::ostream&, const BasePair&);
std::ostream& operator<<(std::ostream&, const Nucleotide&);
std::ostream& operator<<(std::ostream&, const SNP&);

void parse_file(const std::string& filename, DNA&);
void summary(DNA&);

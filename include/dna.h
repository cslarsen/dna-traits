#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <cstdint>

enum Nucleotide {
  NONE, A, G, C, T
};

Nucleotide complement(const Nucleotide& n);

struct Genotype { // should only occupy 5 bits, i.e. 1 byte
  Nucleotide first;
  Nucleotide second;

  Genotype(const Nucleotide& a = NONE,
           const Nucleotide& b = NONE)
    : first(a), second(b)
  {
  }

  friend Genotype operator~(const Genotype& g)
  {
    return Genotype(complement(g.first), complement(g.second));
  }
};

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

// Can most likely pack into a 32-bit value
#pragma pack(2)
struct ID {
  std::uint32_t index;

  enum Type {
    RSID,
    INTERNAL_ID
  } type : 1;

  ID(const char* s) : ID(std::string(s))
  {
  }

  ID(const std::string& s)
  {
    std::stringstream(s) >> *this;
  }

  ID() : index(0), type(RSID)
  {
  }
};

template<>
struct std::hash<ID> {
  std::size_t operator()(const ID& id) const
  {
    return std::hash<std::uint32_t>()(id.index)
         ^ ((id.type & 0x3) << 29);
  }
};

typedef std::uint32_t Position; // [0-9]+

struct Chromosome { // [0-9]+|X|Y|MT
  std::uint8_t type; // 0--22, 23=X, 24=Y, 25=MT

  Chromosome() : type(0)
  {
  }

  Chromosome(const std::string& s)
  {
    if ( isdigit(s[0]) )
      std::stringstream(s) >> type;
    else if ( s == "X" )
      type = 23;
    else if ( s == "Y" )
      type = 24;
    else if ( s == "MT" )
      type = 25;
    else
      throw std::runtime_error("Unknown chromosome type: " + s);
  }

  friend std::ostream& operator<<(std::ostream& o, const Chromosome& c)
  {
    switch ( c.type ) {
    default: return o << c.type;
    case 23: return o << "X";
    case 24: return o << "Y";
    case 25: return o << "MT";
    }
  }

  friend std::istream& operator>>(std::istream& i, Chromosome& c)
  {
    std::string s;
    i >> s;
    c = Chromosome(s);
    return i;
  }
};

struct SNP {
  ID id;
  Genotype genotype;
  Chromosome chromosome;
  Position position;
};

struct DNA {
  std::unordered_map<ID, SNP> snps;

  DNA(const size_t size) : snps(size)
  {
  }

  const Genotype operator[](const ID& id) const
  {
    return snps.at(id).genotype;
  }
};

bool operator==(const Genotype& lhs, const Genotype& rhs);
bool operator==(const ID&, const ID&);
std::istream& operator>>(std::istream&, Genotype&);
std::istream& operator>>(std::istream&, ID&);
std::istream& operator>>(std::istream&, Nucleotide&);
std::ostream& operator<<(std::ostream&, const Genotype&);
std::ostream& operator<<(std::ostream&, const ID& id);
std::ostream& operator<<(std::ostream&, const Nucleotide&);
std::ostream& operator<<(std::ostream&, const SNP&);
void parse_file(const std::string& filename, DNA&);
void summary(const DNA&);

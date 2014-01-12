#include "dna.h"

std::ostream& operator<<(std::ostream& o, const Nucleotide& n)
{
  switch ( n ) {
    case A: return o << 'A';
    case G: return o << 'G';
    case C: return o << 'C';
    case T: return o << 'T';
    default: return o << '-';
  }
}

std::ostream& operator<<(std::ostream& o, const Genotype& bp)
{
  return o << bp.first << bp.second;
}

/**
 * Compare genotypes of same orientation.
 */
bool operator==(const Genotype& lhs, const Genotype& rhs)
{
  return lhs.first == rhs.first && lhs.second == rhs.second;
}

/**
 * Returns the complement nucleotide.
 */
Nucleotide complement(const Nucleotide& n)
{
  switch ( n ) {
    case A: return T;
    case C: return G;
    case G: return C;
    case T: return A;
    case NONE: return NONE;
  }
}

std::string format(const DNA& dna, const RSID& id)
{
  std::stringstream s;
  s << "rs" << id << " " << dna[id];
  return s.str();
}

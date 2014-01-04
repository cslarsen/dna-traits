#include "dna.h"

std::istream& operator>>(std::istream& i, Nucleotide& n)
{
  char ch;
  i >> ch;

  switch ( ch ) {
    case 'A': n = A; break;
    case 'G': n = G; break;
    case 'C': n = C; break;
    case 'T': n = T; break;
    case '-': default: n = NONE; break;
  }

  return i;
}

std::istream& operator>>(std::istream& i, BasePair& bp)
{
  return i >> bp.first >> bp.second;
}

std::ostream& operator<<(std::ostream& o, Nucleotide& n)
{
  switch ( n ) {
    case A: return o << 'A';
    case G: return o << 'G';
    case C: return o << 'C';
    case T: return o << 'T';
    default: return o << '-';
  }
}

std::ostream& operator<<(std::ostream& o, BasePair& bp)
{
  return o << bp.first << bp.second;
}

std::ostream& operator<<(std::ostream& o, SNP& snp)
{
  return
    o << "{"
      << snp.id << " "
      << "genotype=" << snp.genotype << " "
      << "chromosome=" << snp.chromosome << " "
      << "position=" << snp.position
      << "}";
}

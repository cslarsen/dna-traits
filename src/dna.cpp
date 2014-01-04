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
    default: n = MISSING; break;
  }

  return i;
}

std::istream& operator>>(std::istream& i, BasePair& bp)
{
  i >> bp.first;
  i >> bp.second;
  return i;
}

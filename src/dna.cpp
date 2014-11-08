/*
 * Copyright (C) 2014 Christian Stigen Larsen
 * Distributed under the GPL v3 or later. See COPYING.
 */

#include "dna.h"

const SNP NONE_SNP(NO_CHR, 0, NN);

std::ostream& operator<<(std::ostream& o, const Nucleotide& n)
{
  switch ( n ) {
    case A: return o << 'A';
    case C: return o << 'C';
    case D: return o << 'D';
    case G: return o << 'G';
    case I: return o << 'I';
    case NONE: return o << '-';
    case T: return o << 'T';
  }
}

std::ostream& operator<<(std::ostream& o, const Genotype& bp)
{
  return o << bp.first << bp.second;
}

std::ostream& operator<<(std::ostream& o, const Chromosome& chr) {
  if ( chr >= NO_CHR && chr < CHR_MT )
    return o << static_cast<int>(chr);

  switch ( chr ) {
    default: return o;
    case CHR_MT: return o << "MT";
    case CHR_X: return o << "X";
    case CHR_Y: return o << "Y";
  }
}

std::ostream& operator<<(std::ostream& o, const SNP& snp)
{
  return o << snp.genotype << " " << snp.chromosome
    << " " << snp.position;
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
    case D: return D;
    case I: return I;
    case NONE: return NONE;
  }
}

std::string format(const Genome& genome, const RSID& id)
{
  std::stringstream s;
  s << "rs" << id << " " << genome[id];
  return s.str();
}

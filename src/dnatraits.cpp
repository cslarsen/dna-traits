/*
 * Copyright (C) 2014 Christian Stigen Larsen
 * Distributed under the GPL v3 or later. See COPYING.
 */

#include "dnatraits.hpp"

const Genotype AA (A, A);
const Genotype AC (A, C);
const Genotype AG (A, G);
const Genotype AT (A, T);
const Genotype CA (C, A);
const Genotype CC (C, C);
const Genotype CG (C, G);
const Genotype CT (C, T);
const Genotype GA (G, A);
const Genotype GC (G, C);
const Genotype GG (G, G);
const Genotype GT (G, T);
const Genotype NN (NONE, NONE);
const Genotype TA (T, A);
const Genotype TC (T, C);
const Genotype TG (T, G);
const Genotype TT (T, T);
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

Genotype::Genotype(const Nucleotide& a, const Nucleotide& b)
  : first(a), second(b)
{
}

Genotype operator~(const Genotype& g)
{
  return Genotype(complement(g.first),
                  complement(g.second));
}

bool Genotype::operator==(const Genotype& g) const {
  return first == g.first && second == g.second;
}

SNP::SNP(const Chromosome& chr,
    const Position& pos,
    const Genotype& gt) :
  chromosome(chr),
  position(pos),
  genotype(gt)
{
}

SNP::SNP(const SNP& snp) :
  chromosome(snp.chromosome),
  position(snp.position),
  genotype(snp.genotype)
{
}

SNP& SNP::operator=(const SNP& snp) {
  if ( this != &snp ) {
    genotype = snp.genotype;
    chromosome = snp.chromosome;
    position = snp.position;
  }
  return *this;
}

bool SNP::operator==(const Genotype& g) const {
  return genotype == g;
}

Genome::Genome(const size_t size):
  snp(size),
  ychromo(false),
  first(0xffffffff),
  last(0)
{
  snp.set_empty_key(0);
}

const SNP& Genome::operator[](const RSID& id) const
{
  return has(id)? const_cast<SNPMap&>(snp)[id] : NONE_SNP;
}

bool Genome::has(const RSID& id) const
{
  return snp.find(id) != snp.end();
}

struct SNPMapSerializer {
  // Write
  bool operator()(
      FILE *f,
      const std::pair<const RSID, SNP>& v) const
  {
    // Write RSID.
    // TODO: Save in a specific endianness
    if ( fwrite(&v.first, sizeof(v.first), 1, f) != 1 )
      return false;

    // Write Genotype
    if ( fwrite(&v.second, sizeof(v.second), 1, f) != 1 )
      return false;

    return true;
  }

  // Read
  bool operator()(
      FILE *f,
      std::pair<const RSID, SNP>* v) const
  {
    // Read RSID
    // TODO: Convert to native endianness
    if ( fread(const_cast<RSID*>(&v->first), sizeof(v->first), 1, f) != 1 )
      return false;

    // Read Genotype
    if ( fread(const_cast<SNP*>(&v->second),
               sizeof(v->second), 1, f) != 1 )
      return false;

    return true;
  }
};

bool Genome::save(const char* filename) {
  FilePtr f(filename, "wb");
  return snp.serialize(SNPMapSerializer(), f.ptr());
}

bool Genome::load(const char* filename) {
  FilePtr f(filename, "rb");
  return snp.unserialize(SNPMapSerializer(), f.ptr());
}

std::vector<RSID> Genome::intersect(const Genome& genome) const {
  std::vector<RSID> r;

  for ( const auto it : snp )
    if ( genome.has(it.first) )
      r.push_back(it.first);

  return r;
}

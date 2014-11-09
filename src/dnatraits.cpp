/*
 * Copyright (C) 2014 Christian Stigen Larsen
 * Distributed under the GPL v3 or later. See COPYING.
 */

#include <sstream>
#include <sparsehash/dense_hash_map>
#include "dnatraits.hpp"

struct DLL_LOCAL RSIDHash {
  inline std::size_t operator() (const RSID& rsid) const
  {
    return static_cast<std::size_t>(rsid);
  }
};

struct DLL_LOCAL RSIDEq {
  inline bool operator()(const RSID& a, const RSID& b) const
  {
    return a == b;
  }
};

typedef google::dense_hash_map<RSID, SNP, RSIDHash, RSIDEq> SNPMap;

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

struct DLL_LOCAL Genome::GenomeImpl {
  DLL_LOCAL SNPMap snps;

  GenomeImpl(const size_t size) :
    snps(size)
  {
    snps.set_empty_key(0);
  }

  GenomeImpl(const GenomeImpl& g) :
    snps(g.snps)
  {
    snps.set_empty_key(0);
  }

  GenomeImpl& operator=(const GenomeImpl& g)
  {
    if ( this != &g )
      snps = g.snps;

    return *this;
  }

  bool contains(const RSID& rsid) const {
    return snps.find(rsid) != snps.end();
  }

  const SNP& operator[](const RSID& rsid) const {
    return !contains(rsid)? NONE_SNP : const_cast<SNPMap&>(snps)[rsid];
  }

  bool save(FILE* f) {
    return snps.serialize(SNPMapSerializer(), f);
  }

  bool load(FILE* f) {
    return snps.unserialize(SNPMapSerializer(), f);
  }
};


Genome::Genome(const size_t size):
  y_chromosome(false),
  first(0xffffffff),
  last(0),
  pimpl(new GenomeImpl(size))
{
}

Genome::Genome(const Genome& g) :
  y_chromosome(g.y_chromosome),
  first(g.first),
  last(g.last),
  pimpl(new GenomeImpl(*g.pimpl))
{
}

Genome& Genome::operator=(const Genome& g)
{
  if ( this != &g ) {
    *pimpl = *g.pimpl;
    y_chromosome = g.y_chromosome;
    first = g.first;
    last = g.last;
  }
  return *this;
}

Genome::~Genome()
{
  delete pimpl;
}

const SNP& Genome::operator[](const RSID& rsid) const
{
  return (*pimpl)[rsid];
}

bool Genome::has(const RSID& rsid) const
{
  return pimpl->contains(rsid);
}

size_t Genome::size() const
{
  return pimpl->snps.size();
}

double Genome::load_factor() const
{
  return pimpl->snps.load_factor();
}

void Genome::insert(const RSID& rsid, const SNP& snp)
{
  pimpl->snps.insert({rsid, snp});
}

bool Genome::save(const char* filename) {
  FilePtr f(filename, "wb");
  return pimpl->save(f);
}

bool Genome::load(const char* filename) {
  FilePtr f(filename, "rb");
  return pimpl->load(f);
}

std::vector<RSID> Genome::intersect(const Genome& genome) const {
  std::vector<RSID> r;

  for ( const auto it : pimpl->snps )
    if ( genome.has(it.first) )
      r.push_back(it.first);

  return r;
}

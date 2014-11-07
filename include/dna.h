/*
 * Copyright (C) 2014 Christian Stigen Larsen
 * Distributed under the GPL v3 or later. See COPYING.
 */

#ifndef INC_DNATRAITS_H
#define INC_DNATRAITS_H

#include <iostream>
#include <sstream>
#include <string>
#include <cstdint>
#include <sparsehash/dense_hash_map>
#include "fileptr.h"

enum Nucleotide {
  NONE, A, G, C, T, D, I
};

Nucleotide complement(const Nucleotide& n);

// We can get this down to a byte if we want to
#pragma pack(1)
struct Genotype {
  Nucleotide first : 3;
  Nucleotide second : 3;

  Genotype(const Nucleotide& a = NONE,
           const Nucleotide& b = NONE)
    : first(a), second(b)
  {
  }

  friend Genotype operator~(const Genotype& g)
  {
    return Genotype(complement(g.first),
                    complement(g.second));
  }

  bool operator==(const Genotype& g) const {
    return first == g.first && second == g.second;
  }
};

// Some shorthand constants
const Genotype NN (NONE, NONE);
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


enum Chromosome {
  NO_CHR=0,
  CHR1,
  CHR2,
  CHR3,
  CHR4,
  CHR5,
  CHR6,
  CHR7,
  CHR8,
  CHR9,
  CHR10,
  CHR11,
  CHR12,
  CHR13,
  CHR14,
  CHR15,
  CHR16,
  CHR17,
  CHR18,
  CHR19,
  CHR20,
  CHR21,
  CHR22,
  CHR_MT,
  CHR_X,
  CHR_Y
};

typedef std::uint32_t Position;

#pragma pack(1)
struct SNP {
  Chromosome chromosome : 5;
  Position position;
  Genotype genotype;

  SNP(const Chromosome& chr = NO_CHR,
      const Position& pos = 0,
      const Genotype& gt = NN) :
    chromosome(chr),
    position(pos),
    genotype(gt)
  {
  }

  SNP(const SNP& snp) :
    chromosome(snp.chromosome),
    position(snp.position),
    genotype(snp.genotype)
  {
  }

  SNP& operator=(const SNP& snp) {
    if ( this != &snp ) {
      genotype = snp.genotype;
      chromosome = snp.chromosome;
      position = snp.position;
    }
    return *this;
  }

  bool operator==(const Genotype& g) const {
    return genotype == g;
  }
};

typedef std::uint32_t RSID;

struct RSIDHash {
  inline std::size_t operator() (const RSID& rsid) const
  {
    return static_cast<std::size_t>(rsid);
  }
};

struct RSIDEq {
  inline bool operator()(const RSID& a, const RSID& b) const
  {
    return a == b;
  }
};

typedef google::dense_hash_map<RSID, SNP, RSIDHash, RSIDEq> SNPMap;

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

extern const SNP NONE_SNP;

struct DNA {
  SNPMap snp;
  bool ychromo;

  DNA(const size_t size):
    snp(size),
    ychromo(false)
  {
    snp.set_empty_key(0);
  }

  const SNP& operator[](const RSID& id) const
  {
    return has(id)? const_cast<SNPMap&>(snp)[id] : NONE_SNP;
  }

  bool has(const RSID& id) const
  {
    return snp.find(id) != snp.end();
  }

  bool save(const char* filename) {
    FilePtr f(filename, "wb");
    return snp.serialize(SNPMapSerializer(), f.ptr());
  }

  bool load(const char* filename) {
    FilePtr f(filename, "rb");
    return snp.unserialize(SNPMapSerializer(), f.ptr());
  }
};

bool operator==(const Genotype& lhs, const Genotype& rhs);
std::ostream& operator<<(std::ostream&, const Chromosome&);
std::ostream& operator<<(std::ostream&, const Genotype&);
std::ostream& operator<<(std::ostream&, const Nucleotide&);
std::ostream& operator<<(std::ostream&, const SNP&);
std::string format(const DNA&, const RSID&);
void parse_file(const std::string& filename, DNA&);
void summary(const DNA&);

#endif

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

typedef google::dense_hash_map<RSID, Genotype, RSIDHash, RSIDEq> SNPMap;

struct SNPMapSerializer {
  // Write
  bool operator()(
      FILE *f,
      const std::pair<const RSID, Genotype>& v) const
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
      std::pair<const RSID, Genotype>* v) const
  {
    // Read RSID
    // TODO: Convert to native endianness
    if ( fread(const_cast<RSID*>(&v->first), sizeof(v->first), 1, f) != 1 )
      return false;

    // Read Genotype
    if ( fread(const_cast<Genotype*>(&v->second),
               sizeof(v->second), 1, f) != 1 )
      return false;

    return true;
  }
};

struct DNA {
  SNPMap snp;
  bool ychromo;

  DNA(const size_t size):
    snp(size),
    ychromo(false)
  {
    snp.set_empty_key(0);
  }

  const Genotype& operator[](const RSID& id) const
  {
    return has(id)? const_cast<SNPMap&>(snp)[id] : NN;
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
std::ostream& operator<<(std::ostream&, const Genotype&);
std::ostream& operator<<(std::ostream&, const Nucleotide&);
std::string format(const DNA&, const RSID&);
void parse_file(const std::string& filename, DNA&);
void summary(const DNA&);

#endif

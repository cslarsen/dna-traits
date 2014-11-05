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
typedef google::dense_hash_map<RSID, Genotype> SNPMap;

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
};

bool operator==(const Genotype& lhs, const Genotype& rhs);
std::ostream& operator<<(std::ostream&, const Genotype&);
std::ostream& operator<<(std::ostream&, const Nucleotide&);
std::string format(const DNA&, const RSID&);
void parse_file(const std::string& filename, DNA&);
void summary(const DNA&);

#endif

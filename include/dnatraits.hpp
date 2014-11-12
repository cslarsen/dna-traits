/*
 * Copyright (C) 2014 Christian Stigen Larsen
 * Distributed under the GPL v3 or later. See COPYING.
 */

#ifndef INC_DNATRAITS_H
#define INC_DNATRAITS_H

#include <iostream>
#include <string>
#include <cstdint>
#include <vector>
#include "fileptr.hpp"
#include "export.hpp"

typedef std::uint32_t Position;
typedef std::uint32_t RSID;

enum Nucleotide {
  NONE, A, G, C, T, D, I
};

enum Chromosome {
  NO_CHR=0,
  CHR1, CHR2, CHR3, CHR4, CHR5,
  CHR6, CHR7, CHR8, CHR9, CHR10,
  CHR11, CHR12, CHR13, CHR14, CHR15,
  CHR16, CHR17, CHR18, CHR19, CHR20,
  CHR21, CHR22, CHR_MT, CHR_X, CHR_Y
};

// We can get this down to a byte if we want to
#pragma pack(1)
struct DLL_PUBLIC Genotype {
  Nucleotide first : 3;
  Nucleotide second : 3;

  Genotype(const Nucleotide& a = NONE,
           const Nucleotide& b = NONE);

  friend Genotype operator~(const Genotype&);
  bool operator==(const Genotype& g) const;
};

// Some handy constants
extern DLL_PUBLIC const Genotype AA;
extern DLL_PUBLIC const Genotype AC;
extern DLL_PUBLIC const Genotype AG;
extern DLL_PUBLIC const Genotype AT;
extern DLL_PUBLIC const Genotype CA;
extern DLL_PUBLIC const Genotype CC;
extern DLL_PUBLIC const Genotype CG;
extern DLL_PUBLIC const Genotype CT;
extern DLL_PUBLIC const Genotype GA;
extern DLL_PUBLIC const Genotype GC;
extern DLL_PUBLIC const Genotype GG;
extern DLL_PUBLIC const Genotype GT;
extern DLL_PUBLIC const Genotype NN;
extern DLL_PUBLIC const Genotype TA;
extern DLL_PUBLIC const Genotype TC;
extern DLL_PUBLIC const Genotype TG;
extern DLL_PUBLIC const Genotype TT;

#pragma pack(1)
struct DLL_PUBLIC SNP {
  Chromosome chromosome : 5;
  Position position;
  Genotype genotype;

  SNP(const Chromosome& = NO_CHR,
      const Position& = 0,
      const Genotype& = NN);
  SNP(const SNP&);

  SNP& operator=(const SNP&);
  bool operator==(const Genotype&) const;
  bool operator==(const SNP&) const;
  bool operator!=(const SNP&) const;
};

extern DLL_PUBLIC const SNP NONE_SNP;

struct DLL_PUBLIC Genome {
  bool y_chromosome;
  RSID first;
  RSID last;

  Genome(const size_t size);
  Genome(const Genome&);
  Genome& operator=(const Genome&);
  ~Genome();

  const SNP& operator[](const RSID& id) const;
  bool has(const RSID& id) const;
  void insert(const RSID& rsid, const SNP& snp);
  double load_factor() const;
  size_t size() const;
  std::vector<RSID> intersect(const Genome& genome) const;

  bool operator==(const Genome&) const;
  bool operator!=(const Genome&) const;

private:
  struct DLL_LOCAL GenomeImpl;
  DLL_LOCAL GenomeImpl* pimpl;
};

Nucleotide complement(const Nucleotide& n);
std::ostream& operator<<(std::ostream&, const Chromosome&);
std::ostream& operator<<(std::ostream&, const Genotype&);
std::ostream& operator<<(std::ostream&, const Nucleotide&);
std::ostream& operator<<(std::ostream&, const SNP&);
std::string format(const Genome&, const RSID&);
void parse_file(const std::string& filename, Genome&);

#endif

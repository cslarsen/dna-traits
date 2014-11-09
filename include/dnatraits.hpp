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

typedef std::uint32_t Position;
typedef std::uint32_t RSID;

enum Nucleotide {
  NONE, A, G, C, T, D, I
};

// We can get this down to a byte if we want to
#pragma pack(1)
struct Genotype {
  Nucleotide first : 3;
  Nucleotide second : 3;

  Genotype(const Nucleotide& a = NONE,
           const Nucleotide& b = NONE);

  friend Genotype operator~(const Genotype&);
  bool operator==(const Genotype& g) const;
};

enum Chromosome {
  NO_CHR=0,
  CHR1, CHR2, CHR3, CHR4, CHR5,
  CHR6, CHR7, CHR8, CHR9, CHR10,
  CHR11, CHR12, CHR13, CHR14, CHR15,
  CHR16, CHR17, CHR18, CHR19, CHR20,
  CHR21, CHR22, CHR_MT, CHR_X, CHR_Y
};

// Some handy constants
extern const Genotype AA;
extern const Genotype AC;
extern const Genotype AG;
extern const Genotype AT;
extern const Genotype CA;
extern const Genotype CC;
extern const Genotype CG;
extern const Genotype CT;
extern const Genotype GA;
extern const Genotype GC;
extern const Genotype GG;
extern const Genotype GT;
extern const Genotype NN;
extern const Genotype TA;
extern const Genotype TC;
extern const Genotype TG;
extern const Genotype TT;

#pragma pack(1)
struct SNP {
  Chromosome chromosome : 5;
  Position position;
  Genotype genotype;

  SNP(const Chromosome& chr = NO_CHR,
      const Position& pos = 0,
      const Genotype& gt = NN);
  SNP(const SNP& snp);
  SNP& operator=(const SNP& snp);
  bool operator==(const Genotype& g) const;
};

struct Genome {
  bool ychromo;
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
  bool save(const char* filename);
  bool load(const char* filename);
  std::vector<RSID> intersect(const Genome& genome) const;

private:
  struct GenomeImpl;
  GenomeImpl* pimpl;
};

Nucleotide complement(const Nucleotide& n);
std::ostream& operator<<(std::ostream&, const Chromosome&);
std::ostream& operator<<(std::ostream&, const Genotype&);
std::ostream& operator<<(std::ostream&, const Nucleotide&);
std::ostream& operator<<(std::ostream&, const SNP&);
std::string format(const Genome&, const RSID&);
void parse_file(const std::string& filename, Genome&);
void summary(const Genome&);

extern const SNP NONE_SNP;

#endif

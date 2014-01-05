#include <iostream>
#include "dna.h"

using namespace std;

static bool has_key(const DNA& haystack, const ID& needle)
{
  return haystack.snps.find(needle) != haystack.snps.end();
}

/*
 * Compare positively oriented genotype.
 */
static bool eqpos(const SNP& snp, const Genotype& genotype)
{
  return snp.genotype == genotype;
}

/*
 * Flip orientation
 */
static Nucleotide complement(const Nucleotide& n)
{
  switch ( n ) {
    case A: return T;
    case C: return G;
    case G: return C;
    case T: return A;
    default: return NONE;
  }
}

/*
 * Compare negatively oriented genotype.
 */
static bool eqmin(const SNP& snp, const Genotype& genotype)
{
  return snp.genotype.first == complement(genotype.first) &&
         snp.genotype.second == complement(genotype.second);
}

/**
 * Male if we have a Y-chromosome.
 */
string gender(const DNA& dna)
{
  // In case some snips weren't sequenced, query several known ones in the
  // Y-chromosome
  auto male = has_key(dna, "i4000095")
           || has_key(dna, "rs9786543")
           || has_key(dna, "rs9786142")
           || has_key(dna, "rs35679774")
           || has_key(dna, "rs9724556");

  return male? "male" : "female";
}

/**
 * Guess eye-color based on http://snpedia.com/index.php/Gs237/criteria
 * For more info, see http://snpedia.com/index.php/Eye_color
 */
bool blue_eyes(DNA& dna)
{
  return eqpos(dna["rs4778241"], CC)
      && eqpos(dna["rs12913832"], GG)
      && eqpos(dna["rs7495174"], AA)
      && eqpos(dna["rs8028689"], TT)
      && eqpos(dna["rs7183877"], CC)
      && eqmin(dna["rs1800401"], CC);
}

void summary(DNA& dna)
{
  using namespace std;

  cout << "EXAMPLE FINDINGS" << endl << endl;
  cout << "Gender: " << gender(dna) << endl;
  cout << "Blue eye color? " << (blue_eyes(dna)? "Yes" : "No") << endl;
  cout << endl;
}

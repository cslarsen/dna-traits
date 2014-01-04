#include <iostream>
#include "dna.h"

using namespace std;

static bool has_key(const DNA& haystack, const ID& needle)
{
  return haystack.find(needle) != haystack.end();
}

/*
 * Compare positively oriented genotype.
 */
static bool eqpos(const SNP& snp, const Nucleotide& n1, const Nucleotide& n2)
{
  return snp.genotype.first == n1 &&
         snp.genotype.second == n2;
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
static bool eqmin(const SNP& snp, const Nucleotide& n1, const Nucleotide& n2)
{
  return snp.genotype.first == complement(n1) &&
         snp.genotype.second == complement(n2);
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
bool blue_eyes(const DNA& dna)
{
  return eqpos(dna.at("rs4778241"), C, C)
      && eqpos(dna.at("rs12913832"), G, G)
      && eqpos(dna.at("rs7495174"), A, A)
      && eqpos(dna.at("rs8028689"), T, T)
      && eqpos(dna.at("rs7183877"), C, C)
      && eqmin(dna.at("rs1800401"), C, C);
}

void summary(const DNA& dna)
{
  using namespace std;

  cout << "EXAMPLE FINDINGS" << endl << endl;
  cout << "Gender: " << gender(dna) << endl;
  cout << "Blue eye color? " << (blue_eyes(dna)? "Yes" : "No") << endl;
  cout << endl;
}

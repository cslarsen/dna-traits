#include <iostream>
#include "dna.h"

using namespace std;

static bool has_key(const DNA& haystack, const ID& needle)
{
  return haystack.snps.find(needle) != haystack.snps.end();
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
  return dna["rs4778241"] == CC
      && dna["rs12913832"] == GG
      && dna["rs7495174"] == AA
      && dna["rs8028689"] == TT
      && dna["rs7183877"] == CC
      && dna["rs1800401"] == ~CC;
 }

void summary(DNA& dna)
{
  using namespace std;

  cout << "EXAMPLE FINDINGS" << endl << endl;
  cout << "Gender: " << gender(dna) << endl;
  cout << "Blue eye color? " << (blue_eyes(dna)? "Yes" : "No") << endl;
  cout << endl;
}

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
static bool has_ychromo(const DNA& dna)
{
  // In case some snips weren't sequenced, query several known ones in the
  // Y-chromosome
  return has_key(dna, "i4000095")
      || has_key(dna, "rs9786543")
      || has_key(dna, "rs9786142")
      || has_key(dna, "rs35679774")
      || has_key(dna, "rs9724556");
}

/**
 * Has blue eye color? gs237 criteria http://snpedia.com/index.php/Gs237/criteria
 * For more info, see http://snpedia.com/index.php/Eye_color
 */
static bool gs237(const DNA& dna)
{
  return dna[ "rs4778241"] ==  CC
      && dna["rs12913832"] ==  GG
      && dna[ "rs7495174"] ==  AA
      && dna[ "rs8028689"] ==  TT
      && dna[ "rs7183877"] ==  CC
      && dna[ "rs1800401"] == ~CC;
}

void summary(const DNA& dna)
{
  using namespace std;

  cout
    << "Summary of findings:" << endl
    << "  Gender (has Y-chromosome): "
    << (has_ychromo(dna)? "Male" : "Female") << endl
    << "  Blue eyes (criteria gs237): "
    << (gs237(dna)? "Yes" : "No") << endl
    << endl;
}

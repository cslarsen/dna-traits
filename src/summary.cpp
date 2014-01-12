#include <iostream>
#include "dna.h"

using namespace std;

/**
 * Has blue eye color? gs237 criteria http://snpedia.com/index.php/Gs237/criteria
 * For more info, see http://snpedia.com/index.php/Eye_color
 */
static bool gs237(const DNA& dna)
{
  return dna[ 4778241] ==  CC
      && dna[12913832] ==  GG
      && dna[ 7495174] ==  AA
      && dna[ 8028689] ==  TT
      && dna[ 7183877] ==  CC
      && dna[ 1800401] == ~CC;
}

void summary(const DNA& dna)
{
  using namespace std;

  cout
    << "Summary of findings:" << endl
    << "  Gender (has Y-chromosome): "
    << (dna.ychromo? "Male" : "Female") << endl
    << "  Blue eyes (criteria gs237): "
    << (gs237(dna)? "Yes" : "No") << endl
    << endl;
}

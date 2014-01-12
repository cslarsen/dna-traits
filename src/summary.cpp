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

static std::string skin_color_1426654(const DNA& dna)
{
  auto gt = dna[1426654];

  if ( gt == AA )
    return "Probably light-skinned, European ancestry";

  if ( gt == GA || gt == AG )
    return "Mixed African/European ancestry possible";

  if ( gt == GG )
    return "Probably darker-skinned, Asian or African ancestry";

  return "Unknown";
}

void summary(const DNA& dna)
{
  using namespace std;

  cout
    << "SUMMARY" << endl << endl

    << "  Gender:     " << (dna.ychromo? "Male" : "Female")
    << " (has Y-chromosome)" << endl

    << "  Blue eyes?  " << (gs237(dna)? "Yes" : "No")
    << " (based on criteria gs237)" << endl

    << "  Skin color: " << skin_color_1426654(dna) 
    << " (based on rs1426654)" << endl

    << endl;
}

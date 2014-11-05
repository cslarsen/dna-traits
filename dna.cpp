/*
 * Copyright (C) 2014 Christian Stigen Larsen
 * Distributed under the GPL v3 or later. See COPYING.
 */

#include <unistd.h>
#include "dna.h"

int main(int argc, char** argv)
{
  using namespace std;

  try {
    for ( auto n=1; n<argc; ++n ) {
      cout << "Size of Genotype: " << sizeof(Genotype) << endl;

      auto file = argv[n];
      cout << "Reading " << file << " ... ";
      cout.flush();

      DNA dna(1000000);
      parse_file(file, dna);

      cout << "done" << endl; cout.flush();
      cout << "Read " << dna.snp.size() << " unique SNPs" << endl << endl;

#ifdef DEBUG
      cout << "Size of Genotype: " << sizeof(Genotype) << endl
           << "Size of RSID: " << sizeof(RSID) << endl
           << endl;
#endif

      cout << "Example SNPs in this genome:" << endl << endl
           << "  " << format(dna, 7495174) << endl
           << "  " << format(dna, 1805007) << endl
           << "  " << format(dna, 1800401) << endl
           << "  " << format(dna, 28357092) << endl
           << endl;

      summary(dna);
    }

    return 0;
  } catch ( const exception& e) {
    cout << endl
         << "Error: " << e.what() << endl;
    return 1;
  }
}

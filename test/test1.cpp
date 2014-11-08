/*
 * Copyright (C) 2014 Christian Stigen Larsen
 * Distributed under the GPL v3 or later. See COPYING.
 */

#include <unistd.h>
#include "dnatraits.h"

int main(int argc, char** argv)
{
  using namespace std;

  try {
    for ( auto n=1; n<argc; ++n ) {
      auto file = argv[n];
      cout << "Reading " << file << " ... ";
      cout.flush();

      Genome genome(1000000);
      parse_file(file, genome);

      cout << "done" << endl; cout.flush();
      cout << "Read " << genome.snp.size() << " unique SNPs" << endl << endl;

#ifdef DEBUG
      cout << "Size of Genotype: " << sizeof(Genotype) << endl
           << "Size of RSID: " << sizeof(RSID) << endl
           << endl;
#endif

      cout << "Example SNPs in this genome:" << endl << endl
           << "  " << format(genome, 7495174) << endl
           << "  " << format(genome, 1805007) << endl
           << "  " << format(genome, 1800401) << endl
           << "  " << format(genome, 28357092) << endl
           << "  " << format(genome, 12087250) << endl
           << endl;

      summary(genome);

      cout << "Developer info:" << endl
           << "  sizeof(Chromosome) = " << sizeof(Chromosome) << endl
           << "  sizeof(Genotype) = " << sizeof(Genotype) << endl
           << "  sizeof(Nucleotide) = " << sizeof(Nucleotide) << endl
           << "  sizeof(Position) = " << sizeof(Position) << endl
           << "  sizeof(RSID) = " << sizeof(RSID) << endl
           << "  sizeof(SNP) = " << sizeof(SNP) << endl
           << "  load factor = " << genome.snp.load_factor() << endl;
    }

    return 0;
  } catch ( const exception& e) {
    cout << endl
         << "Error: " << e.what() << endl;
    return 1;
  }
}

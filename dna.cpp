#include <unistd.h>
#include "dna.h"

int main(int argc, char** argv)
{
  using namespace std;

  try {
    for ( auto n=1; n<argc; ++n ) {
      auto file = argv[n];
      cout << "Reading " << file << " ... ";
      cout.flush();

      DNA dna;
      parse_file(file, dna);

      cout << "done" << endl << endl;
      cout.flush();

      cout << "Read " << dna.snps.size() << " SNPs" << endl << endl;

#ifdef DEBUG
      cout << "Size of SNP: " << sizeof(SNP) << endl
           << "Size of Chromosome: " << sizeof(Chromosome) << endl
           << "Size of Genotype: " << sizeof(Genotype) << endl
           << "Size of ID: " << sizeof(ID) << endl
           << "Size of Position: " << sizeof(Position) << endl
           << endl;
#endif

      cout << "Example SNPs:" << endl
           << "  " << dna.snps["rs7495174"] << endl
           << "  " << dna.snps["rs1805007"] << endl
           << "  " << dna.snps["rs1800401"] << endl
           << endl;

      summary(dna);
    }

    return 0;
  }catch ( const exception& e) {
    cout << endl << "Error: " << e.what() << endl;
    return 1;
  }
}

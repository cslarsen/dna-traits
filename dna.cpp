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

      cout << "Example SNPs:" << endl
           << dna["rs7495174"] << endl
           << dna["rs1805007"] << endl
           << dna["rs1800401"] << endl
           << endl;

      summary(dna);
    }

    return 0;
  }catch ( const exception& e) {
    cout << endl << "Error: " << e.what() << endl;
    return 1;
  }
}

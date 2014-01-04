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
      auto dna = parse_file(file);
      cout << "done" << endl << endl;

      cout << "Read " << dna.size() << " SNPs" << endl << endl;

      cout << dna["rs7495174"] << endl;
      cout << dna["rs1805007"] << endl;
    }
    return 0;
  }catch ( const exception& e) {
    cout << endl << "Error: " << e.what() << endl;
    return 1;
  }
}

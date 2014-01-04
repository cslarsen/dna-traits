#include "dna.h"

int main(int argc, char** argv)
{
  using namespace std;

  try {
    for ( int n=1; n<argc; ++n ) {
      auto file = argv[n];
      cout << "Reading " << file << endl;
      auto dna = parse_file(file);
      cout << "Done" << endl;
    }
    return 0;
  } catch ( const exception& e) {
    cout << e.what() << endl;
    return 1;
  }
}

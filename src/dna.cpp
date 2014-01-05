#include "dna.h"

std::istream& operator>>(std::istream& i, Nucleotide& n)
{
  char ch;
  i >> ch;

  switch ( ch ) {
    case 'A': n = A; break;
    case 'G': n = G; break;
    case 'C': n = C; break;
    case 'T': n = T; break;
    case '-': default: n = NONE; break;
  }

  return i;
}

std::istream& operator>>(std::istream& i, Genotype& bp)
{
  i >> bp.first;

  // optional second nucleotide
  if ( i.peek()!='\n' && i.peek()!='\r' )
    i >> bp.second;

  return i;
}

std::ostream& operator<<(std::ostream& o, const Nucleotide& n)
{
  switch ( n ) {
    case A: return o << 'A';
    case G: return o << 'G';
    case C: return o << 'C';
    case T: return o << 'T';
    default: return o << '-';
  }
}

std::ostream& operator<<(std::ostream& o, const Genotype& bp)
{
  return o << bp.first << bp.second;
}

std::ostream& operator<<(std::ostream& o, const SNP& snp)
{
  return
    o << "{" << snp.id << " "
      << "genotype=" << snp.genotype << " "
      << "chromosome=" << snp.chromosome << " "
      << "position=" << snp.position << "}";
}

bool operator==(const ID& lhs, const ID& rhs)
{
  return lhs.type==rhs.type && lhs.index==rhs.index;
}

std::istream& operator>>(std::istream& i, ID& id)
{
  using namespace std;
  string s;
  i >> s;

  if ( s[0]=='r' && s[1]=='s' && isdigit(s[2]) ) {
    id.type = ID::RSID;
    stringstream(s.substr(2)) >> id.index;
  } else if ( s[0]=='i' && isdigit(s[1]) ) {
    id.type = ID::INTERNAL_ID;
    stringstream(s.substr(1)) >> id.index;
  } else
    throw runtime_error("Unknown ID type " + s);

  return i;
}

std::ostream& operator<<(std::ostream& o, const ID& id)
{
  switch ( id.type ) {
    case ID::RSID: o << "rs"; break;
    case ID::INTERNAL_ID: o << "i"; break;
    default: break;
  }

  return o << id.index;
}

/**
 * Compare genotypes of same orientation.
 */
bool operator==(const Genotype& lhs, const Genotype& rhs)
{
  return lhs.first == rhs.first && lhs.second == rhs.second;
}

/**
 * Returns the complement nucleotide.
 */
Nucleotide complement(const Nucleotide& n)
{
  switch ( n ) {
    case A: return T;
    case C: return G;
    case G: return C;
    case T: return A;
    case NONE: return NONE;
  }
}

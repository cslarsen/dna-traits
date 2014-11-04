"""
A library for parsing and querying 23andme genomes.
"""

import _dna_traits

class Nucleotide:
    """A single nucleotide."""
    def __init__(self, value):
        self._value = value

    def __str__(self):
        return self._value

    def __repr__(self):
        return "Nucleotide(%s)" % self._value

    def __len__(self):
        if self._value in ["-"]:
            return 0
        else:
            return 1

    def complement(self):
        compl = {"A": "T",
                 "C": "G",
                 "G": "C",
                 "T": "A",
                 "D": "D",
                 "I": "I",
                 "-": "-"}
        return Nucleotide(compl[self._value])

    def __invert__(self):
        return self.complement()

class SNP:
    """A single-nucleotide polymorphism."""

    def __init__(self, genotype, rsid, orientation):
        self._genotype = genotype
        self._rsid = rsid
        self._orientation = orientation

    def _genostr(self):
        return "".join(map(str, self._genotype))

    @property
    def orientation(self):
        """Returns orientation as either -1 or +1."""
        return self._orientation

    def __len__(self):
        return sum(map(len, self._genotype))

    def __getitem__(self, index):
        return self._genotype[index]

    @property
    def rsid(self):
        return self._rsid

    def count(self, nucleotide):
        """Returns number of given nucleotide in this SNP."""
        return self._genostr().count(nucleotide.upper())

    def complement(self):
        genotype = map(lambda n: n.complement(), self._genotype)
        return SNP(genotype, self._rsid, self._orientation)

    def __invert__(self):
        return self.complement()

    @property
    def genotype(self):
        return self._genotype

    def __str__(self):
        return self._genostr()

    def __repr__(self):
        return "SNP(rsid=%s, genotype=%s, orientation=%s)" % (
                    self._rsid, self._genostr(), self._orientation)


class Genome:
    """A genome consisting of SNPs."""

    def __init__(self, genome, orientation):
        self._genome = genome
        self._orientation = orientation

    def _rsid(self, rsid):
        if isinstance(rsid, str):
            return rsid

        if isinstance(rsid, int):
            return "rs%d" % rsid

        raise ValueError("Invalid RSID: %s" % str(rsid))

    @property
    def ychromo(self):
        """True if genome contains a Y-chromosome."""
        return self._genome.ychromo()

    @property
    def male(self):
        """True if genome contains a Y-chromosome."""
        return self.ychromo

    @property
    def female(self):
        """True if genome contains no Y-chromosome."""
        return not self.male

    @property
    def orientation(self):
        return self._orientation

    def __getitem__(self, rsid):
        try:
            rsid = self._rsid(rsid)
            geno = map(Nucleotide, self._genome[rsid])
            return SNP(geno, rsid, self._orientation)
        except KeyError:
            return SNP([Nucleotide("-"), Nucleotide("-")],
                       rsid, self._orientation)

    def snp(self, rsid):
        """Returns SNP with given RSID."""
        return self.__getitem(rsid)

    def __str__(self):
        return "Genome"

    def __contains__(self, rsid):
        try:
            self._genome[self._rsid(rsid)]
            return True
        except KeyError:
            return False

    def __repr__(self):
        return "Genome(len=%d, ychromo=%s, orientation=%s)" % (
                len(self), self.ychromo, self.orientation)

    def __getattr__(self, rsid):
        # Query with genome.rs28357092
        if isinstance(rsid, str) and rsid.startswith("rs"):
            return self.__getitem__(rsid)
        raise AttributeError("Unknown attribute %s" % rsid)

    def __len__(self):
        return len(self._genome)

def parse(filename):
    return Genome(_dna_traits.parse(filename), +1)
"""
A library for parsing and querying 23andme genomes.

Copyright (C) 2014 Christian Stigen Larsen
Distributed under the GPL v3 or later. See COPYING.
"""

import copy
import _dna_traits

def _to_snp(rsid, orientation, value):
    genotype, chromo, position = value
    return SNP(list(genotype), rsid, orientation, chromo, position)

class Nucleotide:
    """A single nucleotide."""
    def __init__(self, value):
        self._value = value

    def __str__(self):
        return self._value

    def __repr__(self):
        return "Nucleotide(%s)" % repr(self._value)

    def __len__(self):
        if self._value in ["-"]:
            return 0
        else:
            return 1

    def __eq__(self, o):
        """Checks if nucleotides are equal."""
        if isinstance(o, Nucleotide):
            return self._value == o._value
        else:
            raise TypeError("Can only compare Nucleotides")

    def complement(self):
        """Returns the complement of this nucleotide."""
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

    def __init__(self,
                 genotype,
                 rsid,
                 orientation,
                 chromosome,
                 position,
                 phased=False):
        self._genotype = genotype
        self._rsid = rsid
        self._orientation = orientation
        self._chromosome = chromosome
        self._position = position
        self._phased = phased

    def _genostr(self):
        return "".join(map(str, self._genotype))

    @property
    def phased(self):
        """Returns whether this SNP is phased or not."""
        return self._phased

    @property
    def chromosome(self):
        """Returns SNP's chromosome."""
        return self._chromosome

    @property
    def position(self):
        """Returns SNP's position in the chromosome."""
        return self._position

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
        """Returns this SNP's RSID."""
        return self._rsid

    def count(self, nucleotide):
        """Returns number of given nucleotide in this SNP."""
        return self._genostr().count(nucleotide.upper())

    def complement(self):
        """Returns this SNP's complement."""
        snp = copy.copy(self)
        snp._genotype = map(lambda n: n.complement(), self._genotype)
        return snp

    def positive(self):
        """Returns SNP with positive orientation."""
        if self.orientation < 0:
            return self.complement()
        else:
            return self

    def negative(self):
        """Returns SNP with negative orientation."""
        if self.orientation > 0:
            return self.complement()
        else:
            return self

    def __eq__(self, obj):
        """Rich-comparison operator for SNPs."""
        if isinstance(obj, SNP):
            return str(self.positive()) == str(obj.positive())
        elif isinstance(obj, str):
            if obj[0]=="~":
                # TODO: Should invert obj instead of us; doesn't matter
                genotype = self.negative()._genostr()
                obj = obj[1:]
            else:
                genotype = self.positive()._genostr()

            if not self.phased:
                return genotype==obj or "".join(reversed(genotype))==obj
            else:
                return genotype==obj
        else:
            return NotImplemented

    def __invert__(self):
        return self.complement()

    @property
    def genotype(self):
        """Returns a list of zero, one or two Nucleotides."""
        return self._genotype

    def __str__(self):
        return self._genostr()

    def __repr__(self):
        s = "SNP(genotype=%s, rsid='%s', orientation=%s, " % (
                repr(self._genotype), self._rsid, self._orientation)
        s += "chromosome=%s, position=%s)" % (self._chromosome,
                self._position)
        return s


class GenomeIterator:
    def __init__(self, genome, start=-1):
        self._genome = genome
        self._rsid = genome.first if start==-1 else start

    def __iter__(self):
        return self

    def next(self):
        while self._rsid not in self._genome:
            self._rsid += 1
            if self._rsid >= self._genome.last:
                raise StopIteration()

        rsid = self._rsid
        self._rsid += 1
        return self._genome[rsid]

class Genome:
    """A genome consisting of SNPs."""

    def __init__(self, genome, orientation):
        self._genome = genome
        self._orientation = orientation

    def _rsid(self, rsid):
        if isinstance(rsid, int):
            return rsid
        elif isinstance(rsid, str) and rsid.lower().startswith("rs"):
            try:
                return int(rsid[2:])
            except ValueError:
                raise ValueError("Invalid RSID: %s" % rsid)
        else:
            raise ValueError("Invalid RSID: %s" % rsid)

    def __iter__(self):
        return GenomeIterator(self)

    def intersect_rsid(self, genome):
        """Find RSIDs that exist in both genomes.

        Returns:
            A list of RSID integers.
        """
        assert(isinstance(genome, Genome))
        return sorted(self._genome.intersect_rsid(genome._genome))

    def intersect_snp(self, genome):
        """Find RSID of SNPs that are equal in both genomes.

        Returns:
            A list of RSID integers.
        """
        assert(isinstance(genome, Genome))
        return sorted(self._genome.intersect_snp(genome._genome))

    @property
    def first(self):
        """Returns lowest RSID in genome."""
        return self._genome.first()

    @property
    def last(self):
        """Returns highest RSID in genome."""
        return self._genome.last()

    @property
    def y_chromosome(self):
        """True if genome contains a Y-chromosome."""
        return self._genome.y_chromosome()

    @property
    def male(self):
        """True if genome contains a Y-chromosome."""
        return self.y_chromosome

    @property
    def female(self):
        """False if genome contains a Y-chromosome."""
        return not self.male

    @property
    def orientation(self):
        """Returns this Genome's orientation as an integer of either -1 or
        +1."""
        return self._orientation

    def __getitem__(self, key):
        """Returns SNP with given RSID.  If RSID is not present, return an
        empty SNP."""
        if isinstance(key, slice):
            return [self[i] for i in xrange(*key.indices(len(self)))]
        elif isinstance(key, str) or isinstance(key, int):
            rsid = self._rsid(key)
            try:
                genotype, chromo, position = self._genome[rsid]
                geno = map(Nucleotide, genotype)
                return _to_snp("rs%d" % rsid, self._orientation, (geno, chromo,
                    position))
            except KeyError:
                return SNP([], "rs%d" % rsid, self._orientation, 0, 0)
        else:
            raise ValueError("Unknown key type %s" % type(key))

    def snp(self, rsid):
        """Returns SNP with given RSID."""
        return self.__getitem__(rsid)

    def __str__(self):
        return "Genome"

    def __contains__(self, rsid):
        try:
            self._genome[self._rsid(rsid)]
            return True
        except KeyError:
            return False

    def __repr__(self):
        return "<Genome: SNPs=%d, y_chromosome=%s, orientation=%s>" % (
                len(self), self.y_chromosome, self.orientation)

    def __getattr__(self, rsid):
        # Query with genome.rs28357092
        if isinstance(rsid, str) and rsid.startswith("rs"):
            return self.__getitem__(rsid)
        raise AttributeError("Unknown attribute %s" % rsid)

    def __len__(self):
        """Returns number of SNPs in this genome."""
        return len(self._genome)

    def __eq__(self, genome):
        return self._genome.eq(genome._genome)

    def __ne__(self, genome):
        return not self.__eq__(genome)

    def match(self, criteria):
        """Match list of (RSID, BasePair) with genome. BasePair should be a
        string with positive orientation.

        Example:
            all(match(genome, [("rs4778241", "CC"), ("rs1291832", "GG")]))
        """
        return (self[rsid]==genotype for rsid, genotype in criteria)


def parse(filename, orientation=+1):
    """Parses a 23andme file and returns a Genome."""
    return Genome(_dna_traits.parse(filename), orientation)

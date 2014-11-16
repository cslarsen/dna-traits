"""
Copyright (C) 2014 Christian Stigen Larsen
Distributed under the GPL v3 or later. See COPYING.
"""

from nucleotide import Nucleotide
from snp import SNP

def _to_snp(rsid, orientation, value):
    genotype, chromo, position = value
    return SNP(list(genotype), rsid, orientation, chromo, position)


class GenomeIterator:
    def __init__(self, genome, start=-1):
        self._genome = genome
        self._rsids = self._genome.rsids
        self._index = 0 if start==-1 else start

    def __iter__(self):
        return self

    def next(self):
        if self._index < len(self._rsids):
            rsid = self._rsids[self._index]
            snp = self._genome[rsid]
            self._index += 1
            return snp
        else:
            raise StopIteration()

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

    @property
    def rsids(self):
        """Returns all RSIDs in this genome."""
        return sorted(self._genome.rsids())

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
        if isinstance(rsid, str) and rsid.lower().startswith("rs"):
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

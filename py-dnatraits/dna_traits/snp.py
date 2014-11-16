"""
Copyright (C) 2014 Christian Stigen Larsen
Distributed under the GPL v3 or later. See COPYING.
"""

import copy

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
    def homozygous(self):
        """Returns whether we have two nucleotides equal to each other."""
        g = self._genostr()
        return len(g)==2 and g[0] == g[1]

    @property
    def heterozygous(self):
        """Returns whether we have two nucleotides different from each other."""
        g = self._genostr()
        return len(g)==2 and g[0] != g[1]

    @property
    def haploid(self):
        """Returns whether SNP is on a haploid chromosome."""
        if self.chromosome in ["X", "Y", "MT"]:
            return True
        genotype = self._genostr().replace("-", "")
        return len(genotype) == 1

    @property
    def sex_chromosome(self):
        """Checks whether associated chromosome is a sex-chromosome."""
        return self.chromosome in ["X", "Y"]

    @property
    def mitochondrial(self):
        """True if mitochondrial DNA."""
        return self.chromosome == "MT"

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

    @property
    def orientation(self):
        """Returns orientation as either -1 or +1."""
        return self._orientation

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
        s = "SNP(genotype='%s', rsid='%s', orientation=%s, " % (
                self._genostr(), self._rsid, self._orientation)
        s += "chromosome=%s, position=%s)" % (self._chromosome,
                self._position)
        return s

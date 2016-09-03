"""
Copyright (C) 2014, 2016 Christian Stigen Larsen
Distributed under the GPL v3 or later. See COPYING.
"""

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

"""
A library for parsing and querying 23andMe genomes.

Copyright (C) 2014 Christian Stigen Larsen
Distributed under the GPL v3 or later. See COPYING.
"""

import _dna_traits
from genome import Genome

def parse(filename, orientation=+1, year=None, ethnicity=None):
    """Parses 23andMe text file and returns a Genome.

    Arguments:
        orientation: Whether genotype is minus (-1) or plus (+1).
        year: Year of birth for individual (optional).
        ethnicity: Ethnicity for individial (optional).
    """
    return Genome(_dna_traits.parse(filename), orientation, year=year,
            ethnicity=ethnicity.lower())

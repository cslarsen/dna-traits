"""
A library for parsing and querying 23andMe genomes.

Copyright (C) 2014 Christian Stigen Larsen
Distributed under the GPL v3 or later. See COPYING.
"""

import _dna_traits
from genome import Genome

def parse(filename, orientation=+1):
    """Parses 23andMe text file and returns a Genome."""
    return Genome(_dna_traits.parse(filename), orientation)

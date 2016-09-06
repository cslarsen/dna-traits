"""
A library for parsing and querying 23andMe genome text files.

Copyright (C) 2014, 2016 Christian Stigen Larsen
Distributed under the GPL v3 or later. See COPYING.
"""

from genome import Genome, GenomeIterator
from match import unphased_match
from nucleotide import Nucleotide
from parse import parse
from snp import SNP

__author__ = "Christian Stigen Larsen"
__copyright__ = "Copyright 2014, 2016 Christian Stigen Larsen"
__email__ = "csl@csl.name"
__license__ = "GNU General Public License v3"
__version__ = "1.0"

__all__ = [
    "Genome",
    "GenomeIterator",
    "Nucleotide",
    "SNP",
    "parse",
    "unphased_match",
]

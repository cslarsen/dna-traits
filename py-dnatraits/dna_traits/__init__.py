"""
A library for parsing and querying 23andMe genomes.

Copyright (C) 2014 Christian Stigen Larsen
Distributed under the GPL v3 or later. See COPYING.
"""

from genome import Genome, GenomeIterator
from nucleotide import Nucleotide
from parse import parse
from snp import SNP
from odds import *
from health import *

__all__ = ["parse", "Nucleotide", "SNP", "Genome", "GenomeIterator"]
__author__ = "Christian Stigen Larsen"
__version__ = "1.0"

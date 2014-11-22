"""Time iterating through an entire genome.

In Python this takes around 9 secs. In C++ it takes a few milliseconds. This
is REALLY bad and needs to be adressed.
"""
import timeit

print("Iterating through entire genome to find Y-chromosomes...")

print timeit.timeit(
    stmt='[s for s in g if s.chromosome=="Y"]',
    setup='import dna_traits as dt; g = dt.parse("../genomes/genome.txt")',
    number=1), "seconds elapsed"

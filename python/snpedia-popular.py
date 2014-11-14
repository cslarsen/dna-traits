#!/usr/bin/env python

"""
Prints various SNPedia popular results.

Copyright (C) 2014 Christian Stigen Larsen
Distributed under the GPL v3 or later. See COPYING.
"""

import sys
import dna_traits as dt

rules = {
    "rs53576": {
        "AA": "Lack of empathy?",
        "AG": "Lack of empathy?",
        "GG": "Optimistic and emphatic, handles stress well"
    },

    "rs1815739": {
        "CC": "Better performing muscles, likely sprinter",
        "CT": "Mix of muscle types, likely sprinter",
        "TT": "Impaired muscle performance, likely endurance athlete"
    },

    "rs6152": {
        "AA": "Won't go bald",
        "AG": "If male, won't go bald",
        "GG": "ABle to go bald"
    },

    "rs1800497-": {
        "CC": "Normal; better avoidance of errors, normal OCD risk,\n" +
              "lower ADHD risk, less alcohol dependence, etc.",
        "CT": "Bad at avoidance of errors, 0.5x lower OCD risk,\n" +
              "higher ADHD risk, etc.",
        "TT": "Bad at avoidance of errors, 0.25x lower OCD,\n" +
              "higher ADHD, alcohol dependence, etc."
    },

    "rs17822931": {
        "CC": "Wet earwax, normal body odour",
        "CT": "Wet earwax, slightly better body odour",
        "TT": "Dry earwax, no body odour, likely Asian descent"
    },

    "rs4680": {
        "AA": "Advantage in memory and attention tasks,\n" +
              "lower pain threshold, enhanced vulnerability to stress,\n" +
              "more efficient at processing information",
        "AG": "Exploratory (see source for details)",
        "GG": "Higher pain threshold, better stress resiliency, modest\n" +
              "reduction in executive cognition performance"
    },
}

def check(filename):
    print(filename)
    genome = dt.parse(filename)

    for rsid, genotypes in sorted(rules.items()):
        negative = False
        if rsid.endswith("-"):
            negative = True
            rsid = rsid[:-1]

        for genotype, descr in genotypes.items():
            snp = genome[rsid]
            if negative:
                snp = ~snp

            if snp == genotype or snp == reversed(genotype):
                print("%-10s %2s: %s" % (rsid, genotype, descr))

if __name__ == "__main__":
    if len(sys.argv) > 1:
        print("NOTE: This is *very* speculative, and may even be erronous!")
        print("Data taken from www.snpedia.com, but code here may be wrong")
    map(check, sys.argv[1:])

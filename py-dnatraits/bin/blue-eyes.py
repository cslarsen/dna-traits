#! /usr/bin/env python

"""
Runs SNPedia criteria gs237 on a genome, determining whether a person (of
European descent) is likely to have blue eyes.

In my experience, this rule set is kinda flaky, but serves her as an example
on how to use the Python API.
"""

import sys
import dna_traits as dt

def gs237(genome):
    """Returns whether SNPedia gs237 criteria matches genome."""
    return all(genome.match((("rs4778241", "CC"),
                             ("rs12913832", "GG"),
                             ("rs7495174", "AA"),
                             ("rs8028689", "TT"),
                             ("rs7183877", "CC"),
                             ("rs1800401", "GG"))))

def blue_eyes(filename):
    """Runs the SNPedia gs237 criterion for determining likelyhood of blue
    eyes."""
    genome = dt.parse(filename)
    result = "Likely" if gs237(genome) else "Unlikely"
    return "%s: Blue eyes (gs237): %s" % (filename, resul)

if __name__ == "__main__":
    for file in sys.argv[1:]:
        print(blue_eyes(file))

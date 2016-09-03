#!/usr/bin/env python

"""
Shows some inferred phenotypes for a genome.

Copyright (C) 2014, 2016 Christian Stigen Larsen
Distributed under the GPL v3 or later. See COPYING.
"""

import sys
import dna_traits as dt

def gs237(genome):
    """SNPedia criteria gs237."""
    return all(genome.match((("rs4778241", "CC"),
                             ("rs12913832", "GG"),
                             ("rs7495174", "AA"),
                             ("rs8028689", "TT"),
                             ("rs7183877", "CC"),
                             ("rs1800401", "GG"))))

def blue_eyes(genome):
    if gs237(genome):
        return "Likely, if European"
    else:
        return "Unlikely?"

def skin_complexion(genome):
    snp = genome.rs1426654

    if snp == "AA":
        return "Likely light-skinned of European descent"

    if snp == "GA" or snp == "AG":
        return "Possibly mixed African-European ancestry"

    if snp == "GG":
        return "Likely dark-skinned of Asian or African ancestry"

    return "Unknown"

def profile(g):
    return {
        "Gender": "Male" if g.male else "Female",
        "Blue eyes": blue_eyes(g),
        "Skin color": skin_complexion(g),
    }

if __name__ == "__main__":
    for file in sys.argv[1:]:
        print(file)
        for k,v in profile(dt.parse(file)).items():
            print("%-18s %s" % (k+":",v))

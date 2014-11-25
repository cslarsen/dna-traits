# -*- encoding: utf-8 -*-

"""
Used to infer some traits.

Use with caution, this code may contain errors!

Copyright (C) 2014 Christian Stigen Larsen
Distributed under the GPL v3 or later. See COPYING.
"""


def bitter_taste(genome):
    """Bitter taste perception."""
    phenotypes = {
        "GG": "Can taste bitter flavours that others can't",
        "CG": "Can taste bitter flavours that others can't",
        "GC": "Can taste bitter flavours that others can't",
        "CC": "Probably can't taste certain bitter flavours"
    }

    snp = str(genome.rs713598)

    if snp in phenotypes:
        return phenotypes[snp]
    else:
        return "<Unknown>"

def traits_report(genome):
    """Computes some traits."""

    checks = [
        bitter_taste,
    ]

    report = {}

    for check in checks:
        try:
            title = check.__doc__[:check.__doc__.index(".")]
            report[title] = check(genome)
        except:
            continue

    return report

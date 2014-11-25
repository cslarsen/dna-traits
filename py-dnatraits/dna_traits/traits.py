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

def alcohol_flush_reaction(genome):
    """Alcohol flush reaction."""
    snp = genome.rs671

    if snp == "AA":
        return "High reaction (no copies of the ALDH2 genej)"
    elif snp == "AG":
        return "Moderate reaction (one copy of the ALDH2 gene)"
    elif snp == "GG":
        return "Little or no reaction (two copies of the ALDH2 gene)"
    else:
        return "<Unknown>"

def earwax_type(genome):
    """Earwax type."""
    snp = genome.rs17822931

    if snp == "CC" or snp == "CT":
        return "Wet earwax (sticky, golden color)"
    elif snp == "TT":
        return "Dry earwax (flaky, pale)"
    else:
        return "<Unknown>"

def eye_color(genome):
    """Eye color."""
    snp = genome.rs12913832

    if genome.ethnicity is not None and genome.ethnicity != "european":
        return "<Can only determine for Europeans>"

    if snp == "AA":
        return "Brown eyes, although 14% have green and 1% have blue"
    elif snp == "AG":
        return "Most likely brown or green, but 7% have blue"
    elif snp == "GG":
        return "Most likely blue, but 30% have green and 1% brown"

def traits_report(genome):
    """Computes some traits."""

    checks = [
        alcohol_flush_reaction,
        bitter_taste,
        earwax_type,
        eye_color,
    ]

    report = {}

    for check in checks:
        try:
            title = check.__doc__[:check.__doc__.index(".")]
            report[title] = check(genome)
        except:
            continue

    return report

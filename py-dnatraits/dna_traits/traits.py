# -*- encoding: utf-8 -*-

"""
Used to infer some traits.

Use with caution, this code may contain errors!

Copyright (C) 2014 Christian Stigen Larsen
Distributed under the GPL v3 or later. See COPYING.
"""

from dna_traits.match import unphased_match

def bitter_taste(genome):
    """Bitter taste perception."""
    return unphased_match(genome.rs713598, {
        "GG": "Can taste bitter flavours that others can't",
        "CG": "Can taste bitter flavours that others can't",
        "GC": "Can taste bitter flavours that others can't",
        "CC": "Probably can't taste certain bitter flavours"})

def alcohol_flush_reaction(genome):
    """Alcohol flush reaction."""
    return unphased_match(genome.rs671, {
        "AA": "Extreme reaction (no copies of the ALDH2 gene)",
        "GG": "Little to no reaction (two copies of the ALDH2 gene)",
        None: "Unable to determine"})

def earwax_type(genome):
    """Earwax type."""
    return unphased_match(genome.rs17822931, {
        "CC": "Wet earwax (sticky, honey-colored)",
        "CT": "Wet earwax (sticky, honey-colored)",
        "TT": "Dry earwax (flaky, pale)",
        None: "Unable to determine"})

def eye_color(genome):
    """Eye color."""
    if genome.ethnicity is not None and genome.ethnicity != "european":
        raise ValueError("Only valid for genomes of European ethnicity")

    return unphased_match(genome.rs12913832, {
        "AA": "Brown eyes, although 14% have green and 1% have blue",
        "AG": "Most likely brown or green, but 7% have blue",
        "GG": "Most likely blue, but 30% have green and 1% brown",
        None: "Unable to determine"})

def lactose_intolerance(genome):
    """Lactose intolerance."""
    return unphased_match(genome.rs4988235, {
        "AA": "Likely lactose tolerant",
        "AG": "Likely lactose tolerant",
        "GG": "Likely lactose intolerant",
        None: "Unable to determine"})

def traits_report(genome):
    """Computes some traits."""

    checks = [
        alcohol_flush_reaction,
        bitter_taste,
        earwax_type,
        eye_color,
        lactose_intolerance,
    ]

    report = {}

    for check in checks:
        title = check.__doc__[:check.__doc__.index(".")]
        try:
            report[title] = check(genome)
        except ValueError, e:
            report[title] = "Error: %s" % e
        except AssertionError, e:
            report[title] = "Error: %s" % e

    return report

# -*- encoding: utf-8 -*-

"""
Used to infer some traits.

Use with caution, this code may contain errors!

Copyright (C) 2014 Christian Stigen Larsen
Distributed under the GPL v3 or later. See COPYING.
"""


def bitter_taste(genome):
    """Bitter taste perception."""
    rs713598 = "".join(sorted(str(genome.rs713598)))
    if rs713598 == "GG" or rs713598 == "CG":
        return "Can taste bitter flavours that others can't"
    if rs713598 == "CC":
        return "Probably cannot taste certain bitter flavours"
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

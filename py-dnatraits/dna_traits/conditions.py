# -*- encoding: utf-8 -*-

"""
Used to infer some inherited conditions.

Use with caution, this code may contain errors!

Copyright (C) 2014 Christian Stigen Larsen
Distributed under the GPL v3 or later. See COPYING.
"""


def arsacs(genome):
    """ARSACS."""
    # Checks for 6954delT mutation in genome.i5012578, search SNPedia for
    # ARSACS for details.
    raise NotImplementedError

def alpha_1_antitrypsin_deficiency(genome):
    """Alpha-1 Antitrypsin Deficiency.

    For more forms, see:
        http://www.skane.se/Upload/Webbplatser/Labmedicin/Verksamhetsområden/Klinisk%20kemi/Analyser/Malmo/B-DNA-alfa1-antitrypsin%20genotyp%20variant%20S%20och%20Z%20Taqman%20allel-diskriminering%207900%20HT.pdf
    """
    if "rs17580" not in genome:
        return "<Unknown, rs17580 is missing>"

    if "rs28929474" not in genome:
        return "<Unknown, rs28929474 is missing>"

    rs17580 = "".join(sorted(str(genome.rs17580)))
    rs28929474 = "".join(sorted(str(genome.rs28929474)))

    # Some variants below are ambigous but could be determined if the
    # genotype was phased.

    # TODO: Check if genome.phased is True and use that to resolve
    # ambiguities.

    # TODO: The table below is a bit messy (and may contain errors). Fix it!

    variants = {
        "AACC": "SS: Two copies of the S form of the SERPINA1 gene",
        "AACT": "SZ: Two bad copies of the SERPINA1 gene",
        "AATC": "SZ: Two bad copies of the SERPINA1 gene",
        "AATT": "<Unknown, but uncommon variants of the SERPINA1 gene>",
        "ATCC": "MS: One copy of the S form of the SERPINA1 gene",
        "ATCT": "At least one bad form of the SERPINA1 gene, needs phasing",
        "ATTC": "At least one bad form of the SERPINA1 gene, needs phasing",
        "ATTT": "Two or more bad forms of the SERPINA1 gene",
        "TACC": "MS: One copy of the S form of the SERPINA1 gene",
        "TACT": "At least one bad form of the SERPINA1 gene, needs phasing",
        "TATC": "At least one bad form of the SERPINA1 gene, needs phasing",
        "TATT": "Two or more bad forms of the SERPINA1 gene",
        "TTCC": "MM: Normal form of the SERPINA1 gene",
        "TTCT": "MZ: One copy of Z form of the SERPINA1 gene",
        "TTTC": "MZ: One copy of Z form of the SERPINA1 gene",
        "TTTT": "ZZ: Two copies of the Z form of the SERPINA1 gene",
    }

    try:
        return variants[rs17580 + rs28929474]
    except KeyError:
        return "<Unknown variant: %s>" % (rs17580 + rs28929474)

def inherited_conditions(genome):
    """Computes some inherited conditions."""

    checks = [
        alpha_1_antitrypsin_deficiency,
        arsacs,
    ]

    report = {}

    for check in checks:
        try:
            title = check.__doc__[:check.__doc__.index(".")]
            report[title] = check(genome)
        except NotImplementedError:
            continue

    return report

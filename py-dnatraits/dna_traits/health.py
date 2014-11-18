"""
Used to infer some health related reports.

Use with caution, this code may contain errors!

Copyright (C) 2014 Christian Stigen Larsen
Distributed under the GPL v3 or later. See COPYING.
"""

def apoe_variants(genome):
    """APOE-variants (Alzheimer's)."""

    genotypes = "".join(sorted(str(genome.rs429358)))
    genotypes += "".join(sorted(str(genome.rs7412)))

    variants = {
        "CCCC": "e4/e4",
        "CCCT": "e1/e4",
        "CCTT": "e1/e1",
        "CTCC": "e3/e4",
        "CTCT": "e1/e3 or e2/e4", # ambiguous
        "CTTT": "e1/e2",
        "TTCC": "e3/e3",
        "TTCT": "e2/e3",
        "TTTT": "e2/e2",
    }

    return variants[genotypes]

def rheumatoid_arthritis_risk(genome):
    """Rheumatoid arthritis."""

    odds = 0

    # FIXME: Fix the odds calculation, it's a complete mess right now
    # (attempt to use Mantel-Haenszel instead).
    #
    # We currently just give a score for each risk allele instead and give
    # an thumbs up / down rating.

    # These are not applicable for Asians
    if ethnicity == "european":
        odds += genome.rs6457617.count("T")
        if genome.rs2476601 == "GG": odds -= 1

    if genome.rs3890745 == "CC": odds += -1
    if genome.rs2327832 == "AG": odds += -1

    # Only Europeans
    # http://www.ncbi.nlm.nih.gov/pmc/articles/PMC2636867/
    odds += genome.rs3761847.count("G")

    if genome.rs7574865 == "TT": odds += 1
    if genome.rs1569723 == "AA": odds += 1
    if genome.rs13031237 == "GT": odds += 1

    # TODO: Implement rest, ignore scores, just give a "low/medium/high"
    # odds.

    if odds <= 2:
        return "low risk"
    elif odds <= 4:
        return "medium risk"
    else:
        return "high risk"

def health_reports(genome):
    """Computes some health-related risks."""

    checks = [
        apoe_variants,
        rheumatoid_arthritis_risk,
    ]

    report = {}

    for check in checks:
        title = check.__doc__[:check.__doc__.index(".")]
        report[title] = check(genome)

    return report

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
    raise NotImplementedError()

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

def chronic_kidney_disease(genome):
    """Chrinic kidney disease.

    rs4293393 european AA = 1.08
    rs7805747 european GG = 0.96, AG = 1.14

    Citations:
        http://www.ncbi.nlm.nih.gov/entrez/query.fcgi?cmd=Search&db=PubMed&term=21082022
        http://www.ncbi.nlm.nih.gov/entrez/query.fcgi?cmd=Search&db=PubMed&term=20686651
        http://www.ncbi.nlm.nih.gov/entrez/query.fcgi?cmd=Search&db=PubMed&term=19430482
    """
    raise NotImplementedError()
    # TODO: See studies, find odds ratios and implement

    # From http://www.snpedia.com/index.php/Rs4293393
    risk = 1.0
    risk -= 0.24 * ~genome.rs4293393.count("G")

    # 1.18 [1.12-1.25], P-val 4E-12
    risk -= genome.rs7805747.count("A")*1.18

    return risk

def restless_leg_syndrome(genome):
    """Restless leg syndrome.

    Only for European ancestry.

    rs3923809 AA 1.26
              AG 0.74

    Citations:
        http://www.ncbi.nlm.nih.gov/entrez/query.fcgi?cmd=Search&db=PubMed&term=17634447
        http://www.ncbi.nlm.nih.gov/entrez/query.fcgi?cmd=Search&db=PubMed&term=17637780
        http://www.ncbi.nlm.nih.gov/entrez/query.fcgi?cmd=Search&db=PubMed&term=11340155
    """
    if genome.rs3923809 == "GG":
        return "Normal risk"
    elif genome.rs3923809 == "AG" or genome.rs3923809 == "GA":
        return "Slightly increased risk"
    elif genome.rs3923809 == "AA":
        return "Twice as high risk for developing"
    else:
        return "<Unknown genotype for rs3923809 %s>" % genome.rs3923809

def health_report(genome):
    """Computes some health-related risks."""

    checks = [
        apoe_variants,
        chronic_kidney_disease,
        restless_leg_syndrome,
        rheumatoid_arthritis_risk,
    ]

    report = {}

    for check in checks:
        try:
            title = check.__doc__[:check.__doc__.index(".")]
            report[title] = check(genome)
        except NotImplementedError:
            continue

    return report

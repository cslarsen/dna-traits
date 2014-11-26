# -*- encoding: utf-8 -*-

"""
Used to infer some traits.

Use with caution, this code may contain errors!

Copyright (C) 2014 Christian Stigen Larsen
Distributed under the GPL v3 or later. See COPYING.
"""

from dna_traits.match import unphased_match

def _assert_european(genome):
    """If ethnicity is set, make sure it's European."""
    if genome.ethnicity not in [None, "european"]:
        raise ValueError("Only applicable to Europeans")

def bitter_taste(genome):
    "Bitter taste perception."
    return unphased_match(genome.rs713598, {
        "CC": "Probably can't taste certain bitter flavours",
        "CG": "Can taste bitter flavours that others can't",
        "GG": "Can taste bitter flavours that others can't",
        None: "Unable to determine"})

def alcohol_flush_reaction(genome):
    "Alcohol flush reaction."
    return unphased_match(genome.rs671, {
        "AA": "Extreme reaction (no copies of the ALDH2 gene)",
        "AG": "Moderate reaction (one copy of the ALDH2 gene)",
        "GG": "Little to no reaction (two copies of the ALDH2 gene)",
        None: "Unable to determine"})

def earwax_type(genome):
    "Earwax type."
    return unphased_match(genome.rs17822931, {
        "CC": "Wet earwax (sticky, honey-colored)",
        "CT": "Wet earwax (sticky, honey-colored)",
        "TT": "Dry earwax (flaky, pale)",
        None: "Unable to determine"})

def eye_color(genome):
    "Eye color."
    _assert_european(genome)
    return unphased_match(genome.rs12913832, {
        "AA": "Brown eyes, although 14% have green and 1% have blue",
        "AG": "Most likely brown or green, but 7% have blue",
        "GG": "Most likely blue, but 30% have green and 1% brown",
        None: "Unable to determine"})

def lactose_intolerance(genome):
    "Lactose intolerance."
    return unphased_match(genome.rs4988235, {
        "AA": "Likely lactose tolerant",
        "AG": "Likely lactose tolerant",
        "GG": "Likely lactose intolerant",
        None: "Unable to determine"})

def malaria_resistance(genome):
    "Malaria resistance (Duffy antigen)."
    return unphased_match(genome.rs2814778, {
        "CC": "Likely resistant to P. vivax",
        "CT": "Likely to have some resistance to P. vivax",
        "TT": "Likely not resistant to P. vivax",
        None: "Unable to determine"})

def male_pattern_baldness(genome):
    """Male pattern baldness.

    Studies:
        http://www.ncbi.nlm.nih.gov/entrez/query.fcgi?cmd=Search&db=PubMed&term=18849991
        http://www.ncbi.nlm.nih.gov/entrez/query.fcgi?cmd=Search&db=PubMed&term=15902657
        http://www.ncbi.nlm.nih.gov/entrez/query.fcgi?cmd=Search&db=PubMed&term=18849994
    """
    raise NotImplementedError()

    # X-chromosome: rs6625163, A->G is risk mutation (OR 1.17)
    # rs6113491, A->C is risk mutation (AA has OR 1.77)
    # TODO: Attempt to match ORs

def norovirus_resistance(genome):
    """Norovirus resistance (most common strain)."""
    return unphased_match(genome.rs601338, {
        "AA": "Resistant to most common strain",
        "AG": "Likely not resistant to most common strain",
        "GG": "Likely not resistant to most common strain",
        None: "Unable to determine"})

def muscle_performance(genome):
    """Muscle performance."""
    return unphased_match(genome.rs1815739, {
        "CC": "Likely sprinter, perhaps endurance athlete (two copies)",
        "CT": "Likely sprinter, perhaps endurance athlete (one copy)",
        "TT": "Unlikely sprinter, but likely endurance athlete (no copies)",
        None: "Unable to determine"})

def smoking_behaviour(genome):
    """Smoking behaviour."""
    _assert_european(genome)
    return unphased_match(genome.rs1051730, {
        "AA": "Likely to smoke more than average",
        "AG": "Likely to smoke a little bit more than average",
        "GG": "Likely to smoke typical amount of cigarettes per day",
        None: "Unable to determine"})

def red_hair(genome):
    """Hair color; odds for red hair."""
    return unphased_match(genome.rs1805007, {
        "CC": "Typical odds for red hair",
        "CT": "Substantially increased odds for red hair",
        "TT": "Greatly increased odds for red hair",
        None: "Unable to determine"})

def blond_vs_brown_hair(genome):
    """Hair color; blond versus brown."""
    return unphased_match(genome.rs1667394, {
        "CC": "Greatly decreased odds of having blond hair vs. brown",
        "CT": "Decreased odds of having blond hair vs. brown",
        "TT": "Typical odds of having blond hair vs. brown hair",
        None: "Unable to determine"})

def pain_sensitivity(genome):
    """Pain sensitivity."""
    return unphased_match(genome.rs6269, {
        "AA": "Increased sensitivity to pain",
        "AG": "Typical sensitivity to pain",
        "GG": "Less sensitive to pain",
        None: "Unable to determine"})

def caffeine_metabolism(genome):
    """Caffeine metabolism."""
    _assert_european(genome)
    return unphased_match(genome.rs762551, {
        "AA": "Fast metabolizer",
        "AC": "Slow metabolizer",
        "CC": "Slow metabolizer",
        None: "Unable to determine"})

def heroin_addiction(genome):
    """Heroin addiction."""
    _assert_european(genome)
    return unphased_match(genome.rs1799971, {
        "AA": "Typical odds of addiction",
        "AG": "Higher odds of addiction",
        "GG": "Higher odds of addiction",
        None: "Unable to determine"})

def hair_curl(genome):
    _assert_european(genome)
    return unphased_match(genome.rs17646946, {
        "AA": "Straighter hair on average",
        "AG": "Straighter hair on average",
        "GG": "Slightly curlier hair on average"})

def traits_report(genome):
    """Computes some traits."""

    checks = [
        alcohol_flush_reaction,
        bitter_taste,
        blond_vs_brown_hair,
        caffeine_metabolism,
        earwax_type,
        eye_color,
        hair_curl,
        heroin_addiction,
        lactose_intolerance,
        malaria_resistance,
        male_pattern_baldness,
        muscle_performance,
        norovirus_resistance,
        pain_sensitivity,
        red_hair,
        smoking_behaviour,
    ]

    report = {}

    for check in checks:
        if check.__doc__ is not None:
            title = check.__doc__[:check.__doc__.index(".")]
        else:
            title = check.__name__.replace("_", " ").capitalize()
        try:
            report[title] = check(genome)
        except ValueError, e:
            report[title] = "Error: %s" % e
        except AssertionError, e:
            report[title] = "Error: %s" % e
        except NotImplementedError:
            continue

    return report

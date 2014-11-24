# -*- encoding: utf-8 -*-

"""
Used to infer some health related reports.

Use with caution, this code may contain errors!

Copyright (C) 2014 Christian Stigen Larsen
Distributed under the GPL v3 or later. See COPYING.
"""

from dna_traits import odds

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

    try:
        return variants[genotypes]
    except KeyError:
        return "<Unknown variant: %s>" % genotypes

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
    if genome.ethnicity == "european":
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
    """Chronic kidney disease (CKD).

    Citations:
        http://www.ncbi.nlm.nih.gov/entrez/query.fcgi?cmd=Search&db=PubMed&term=21082022
        http://www.ncbi.nlm.nih.gov/entrez/query.fcgi?cmd=Search&db=PubMed&term=20686651
        http://www.ncbi.nlm.nih.gov/entrez/query.fcgi?cmd=Search&db=PubMed&term=19430482
    """
    # List of (OR, CI, P-value, variance inflaction factor)
    ors = []

    # Taken from the following GWAS:
    # http://www.ncbi.nlm.nih.gov/pmc/articles/PMC2912386/#pgen.1001039-Gretarsdottir2
    if genome.ethnicity is None or genome.ethnicity=="european":
        # TODO: Find out if the OR is per T-allele or just for the existence
        # of one. Here I just see if there is one or more.
        if genome.rs4293393.negative().count("T") > 0:
            if genome.year_of_birth is None:
                ors.append((1.25, 0.95, 4.1e-10, 1.15))
            else:
                # Stratified OR. Honestly, I think the P-values seem WAY too
                # high for births later than 1940.
                if genome.year_of_birth < 1920:
                    ors.append((1.19, 0.95, 0.045, 1.15))
                elif genome.year_of_birth < 1930:
                    ors.append((1.31, 0.95, 4.1e-7, 1.15))
                elif genome.year_of_birth < 1940:
                    ors.append((1.28, 0.95, 3.1e-5, 1.15))
                elif genome.year_of_birth < 1950:
                    ors.append((1.16, 0.95, 0.12, 1.15))
                else:
                    ors.append((1.09, 0.95, 0.57, 1.15))

    # Taken from:
    # http://www.ncbi.nlm.nih.gov/pmc/articles/PMC2997674/
    if genome.ethnicity is None or genome.ethnicity=="european":
        # Table 3:
        if genome.rs7805747.count("A") > 0:
            ors.append((1.19, 0.95, 4.2e-12, None))
        pass

    if len(ors) > 0:
        ORs = [d[0] for d in ors]
        pvals = [d[2] for d in ors]
        OR_mh, se, pp = odds.pooled_or(zip(ORs, pvals), 1.15)
        rr = odds.relative_risk(OR_mh, 0.034)
        return "%.2f relative risk, %.2f odds ratio (%d markers)" % (rr, OR_mh, len(ors))
    else:
        return "<No data>"

    """
    rs4293393 AA european OR 1.08 (adjusted)
    rs7805747 AG european OR 1.14 (adjusted)
    rs7805747 AG european OR 0.96 (adjusted)

    From:
    http://www.plosgenetics.org/article/fetchObject.action?uri=info%3Adoi%2F10.1371%2Fjournal.pgen.1001039&representation=PDF
    rs4293393-T associated with CKD, OR=1.25, P=4.1e-10. Association
    stronger with older age groups. CI=1.17-1.35 (95%), N=3203 (no of cases)
    Disregard year of birth (stronger association with old age).
    See Köttgen.

    Note sure if PER T-allele. Only think it's the existence of this allele.
    Also, is it minus orientation?

    SNPedia says, each G at this allele (actually A because snpedia uses
    minus orientation) decrease risk with 24%.

    From dbsnp, http://www.ncbi.nlm.nih.gov/SNP/snp_ref.cgi?rs=4293393
    it seems that the illumina hapmap300 used in the study uses minus
    orientation, because it can only have C/T alleles, while 23andme reports
    the A-allele. So this means that A(+) or T(-) is the risk allele.
    The reverse version (G+, C-) is protective of CKD actually.

    Says:
    Association analysis

    For case-control association analysis, e.g. for CKD and kidney stones,
    we utilized a standard likelihood ratio statistic, implemented in the
    NEMO software [32] to calculate two-sided P values and odds ratios (ORs)
    for each individual allele, assuming a multiplicative model for risk,
    i.e. that the risk of the two alleles carried by a person multiplies
    [36]. Allelic frequencies, rather than carrier frequencies, are
    presented for the markers and P values are given after adjustment for
    the relatedness of the subjects. When estimating genotype specific OR,
    genotype frequencies in the population were estimated assuming
    Hardy-Weinberg equilibrium.

    Results from multiple case-control groups were combined using a
    Mantel-Haenszel model [37] in which the groups were allowed to have
    different population frequencies for alleles, haplotypes and genotypes
    but were assumed to have common relative risks.

    For the quantitative trait association analysis, e.g. for SCr and
    cystatin C, we utilized a robust linear regression based on an M
    estimator [38] as implemented in the rlm function of the R software
    package [39]. An additive model for SNP effects was assumed in all
    instances. All associations with quantitative traits were performed
    adjusting for age and sex.
    """

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

def scleroderma(genome):
    """Scleroderma (limited cutaneous type)."""
    # TODO: Implement odds ratios, find all alleles
    if genome.ethnicity is None or genome.ethnicity == "european":
        if genome.rs7574865 == "TT":
            return "Higher odds"
        if genome.rs7574865.count("T") > 0:
            return "Slight risk"
        return "<Unknown>"
    else:
        return "<Unknown for this ethnicity>"

def health_report(genome):
    """Computes some health-related risks."""

    checks = [
        apoe_variants,
        chronic_kidney_disease,
        restless_leg_syndrome,
        rheumatoid_arthritis_risk,
        scleroderma,
    ]

    report = {}

    for check in checks:
        try:
            title = check.__doc__[:check.__doc__.index(".")]
            report[title] = check(genome)
        except NotImplementedError:
            continue

    return report

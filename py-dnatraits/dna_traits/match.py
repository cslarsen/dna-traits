"""
Contains matching functions.

Copyright (C) 2014 Christian Stigen Larsen
Distributed under the GPL v3 or later. See COPYING.
"""

def unphased_match(snp, phenotypes):
    """Match SNP with unphased genotypes and return phenotype.

    Disregards phasing when comparing genotypes.

    Example:
        unphased_match(genome.rs4988235, {
            "AA": "Likely lactose tolerant",
            "AG": "Likely lactose tolerant",
            "GG": "Likely lactose intolerant",
            None: "Unknown genotype"})
    """
    genotype = str(snp)
    if genotype in phenotypes:
        return phenotypes[genotype]

    genotype = "".join(reversed(str(snp)))
    if genotype in phenotypes:
        return phenotypes[genotype]

    # Default return value
    if None in phenotypes:
        return phenotypes[None]

    raise KeyError("No genotype '%s' in phenotype map." % str(snp))

#!/usr/bin/env python

"""
Determines haplotype for a child-parent relationship.

TODO:

Remember that some genotype calls may have failed for either of parent or
child, so be selective about which SNP to create a haplotype out of (this is
especially for cases with minus markers).
"""

import sys
import dna_traits as dt

def haplotype(child, parent):
    """Given a parent and child, extract the haplotype where possible.

    Return:
        Generator yielding haploid SNPs.
    """

    def haploid(snp):
        """Return haploid SNP using the first nucleotide."""
        return dt.SNP([snp.genotype[0]],
                      snp.rsid,
                      orientation=snp.orientation,
                      chromosome=snp.chromosome,
                      position=snp.position)

    # Generate SNPs that are found in both genomes
    common = ((child[rsid], parent[rsid]) for rsid in
              child.intersect_rsid(parent))

    for c, p in common:
        if c.haploid: # haploid chromosome (X,Y,MT)
            yield haploid(c)

        elif c.homozygous: # child: AA,GG,CC,TT,DD,II,--
            yield haploid(c)

        elif p.homozygous: # parent: AA,GG,CC,TT,DD,II,--
            yield haploid(p)

        elif p.heterozygous: # impossible to determine with one parent
            continue

if __name__ == "__main__":
    if len(sys.argv) < 3:
        print("Usage: haplotype child parent")
        sys.exit(1)

    def parse(name):
        sys.stdout.write("Parsing %s" % name)
        sys.stdout.flush()
        return dt.parse(name)

    child = parse(sys.argv[1])
    parent = parse(sys.argv[2])

    print("First 10 haplotypes:")
    for index, snp in enumerate(haplotype(child, parent)):
        print(repr(snp))
        if index == 9:
            break

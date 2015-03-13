#!/usr/bin/env python

import csv
import sys

def parse(filename):
    """A fallback 23andMe DNA parser."""
    snp = {}

    with open(filename, "rt") as f:
        for line in f:
            if line[0] != '#':
                break

        for row in csv.reader(f, delimiter="\t"):
            rsid, chromosome, position, genotype = row
            snp[rsid] = (chromosome, int(position), genotype)

    return snp

if __name__ == "__main__":
    if len(sys.argv) <= 1:
        print("Usage: %s [ genome(s) ]" % sys.argv[0])
        sys.exit(1)

    for name in sys.argv[1:]:
        sys.stdout.write("Reading %s ... " % name)
        sys.stdout.flush()

        snp = parse(name)

        sys.stdout.write("%d SNPs\n" % len(snp))

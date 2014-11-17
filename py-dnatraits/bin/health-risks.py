"""
Shows some health related reports.

Use with caution, this code may contain errors!

Copyright (C) 2014 Christian Stigen Larsen
Distributed under the GPL v3 or later. See COPYING.
"""

import sys
import dna_traits as dt

def apoe_variants(genome):
    """Returns APOE variants associated with Alzheimer's risk."""

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
    """Returns risk for rheumatoid arthritis."""

    risk = 0

    if genome.rs6457617 == "TT": risk += 2.0
    if genome.rs2476601 == "GG": risk -= 0.8

    # TODO: Implement rest, ignore scores, just give a "low/medium/high"
    # risk.

    if risk > 2:
        return "high risk?"
    elif risk > 1:
        return "medium risk?"
    else:
        return "low risk?"

def health_reports(genome):
    report = {
        "APOE-variants (Alzheimer's)": apoe_variants(genome),
        "Rheumatoid arthritis": rheumatoid_arthritis_risk(genome),
    }

    return report

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Usage: health-reports genome(s)")
        sys.exit(1)

    for filename in sys.argv[1:]:
        genome = dt.parse(filename)
        result = health_reports(genome)

        print(filename)

        for title, description in sorted(result.items()):
            print("%s: %s" % (title, description))

        print("")

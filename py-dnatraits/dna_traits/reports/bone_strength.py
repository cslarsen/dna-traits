"""
Scores your bone strength based on SNPs in the WNT16-gene.

The algorithm here was taken from
https://github.com/23andMe/api-example-go/blob/master/app.go

For the paper _that_ is based on, see
http://www.plosgenetics.org/article/info:doi/10.1371/journal.pgen.1002745

Copyright (C) 2014, 2016 Christian Stigen Larsen
Distributed under the GPL v3 or later. See COPYING.
"""

import sys
import dna_traits as dt

def bone_strength(genome):
    """Returns bone strength indicators."""
    return {
        "Cortical strength": (4,
            4 - genome.rs9525638.count("T") 
              - genome.rs2707466.count("C")),
        "Forearm bone mineral density (BMD)": (4,
            4 - genome.rs2908004.count("G")
              - genome.rs2707466.count("C")),
        "Lower forearm risk": (6,
            6 - genome.rs7776725.count("C")
              - genome.rs2908004.count("G")
              - genome.rs2707466.count("C"))}

def genome_report(genome):
    scores = bone_strength(genome)
    print("Bone strength (higher is stronger)")

    # Add totals
    max_total = sum(p for p,_ in scores.values())
    total = sum(p for _,p in scores.values())
    scores["Total score"] = (max_total, total)

    for title, (max_score, score) in scores.items():
      print("  %18s: %2d of %2d" % (title, score, max_score))

    print("")

    for rsid in ["rs2707466", "rs2908004", "rs7776725", "rs9525638"]:
        if rsid not in genome:
            print(("Note: %s wasn't in this genome; scores may be a bit " +
                  "too high") % rsid)

def report_bone_strength(files):
  if not files:
    print("Usage: bone-strength [ file(s) ]")
    print("The files must be human genomes in 23andMe-format.")
    return 1

  for file in files:
    genome = dt.parse(file)
    scores = bone_strength(genome)

    print("Bone-strength estimated from genome %s" % file)
    print("(Higher points is stronger bone)\n")

    # Add totals
    max_total = sum(p for p,_ in scores.values())
    total = sum(p for _,p in scores.values())
    scores["Total score"] = (max_total, total)

    for title, (max_score, score) in scores.items():
      print("  %18s: %2d of %2d" % (title, score, max_score))

    print("")

    for rsid in ["rs2707466", "rs2908004", "rs7776725", "rs9525638"]:
        if rsid not in genome:
            print(("Note: %s wasn't in this genome; scores may be a bit " +
                  "too high") % rsid)

if __name__ == "__main__":
  try:
    sys.exit(report_bone_strength(sys.argv[1:]))
  except Exception, e:
    print("Error: %s" % e.message)

#!/usr/bin/env python

"""
Scores your bone strength based on SNPs in the WNT16-gene.

The algorithm here was taken from
https://github.com/23andMe/api-example-go/blob/master/app.go

For the paper _that_ is based on, see
http://www.plosgenetics.org/article/info:doi/10.1371/journal.pgen.1002745

Copyright (C) 2014 Christian Stigen Larsen
Distributed under the GPL v3 or later. See COPYING.
"""

import sys
import dna_traits as dt

class SNPs(object):
  """Helper-class for looking up SNPs."""

  def __init__(self, genome):
    self.genome = genome

  def __getattr__(self, name):
    # Interpcet RSIDs
    if name.startswith("rs"):
      try:
        return self.genome[name]
      except KeyError:
        return ""
    else:
      raise AttributeError("Invalid RSID %s" % name)

  def male(self):
    """Return True if genome belongs to a male."""
    return self.genome.ychromo()

  def has(self, rsid):
    """Return True if RSID is found in genome."""
    try:
      self.genome[rsid]
      return True
    except KeyError:
      return False

def bone_strength(snp):
  """Returns bone strength indicators."""
  return {"Cortical strength": (4, 4 - snp.rs9525638.count("T") 
                                     - snp.rs2707466.count("C")),
                "Forearm BMD": (4, 4 - snp.rs2908004.count("G")
                                     - snp.rs2707466.count("C")),
         "Lower forearm risk": (6, 6 - snp.rs7776725.count("C")
                                     - snp.rs2908004.count("G")
                                     - snp.rs2707466.count("C"))}

def main(files):
  if not files:
    print("Usage: bone-strength [ file(s) ]")
    print("The files must be human genomes in 23andMe-format.")
    return 1

  for file in files:
    genome = SNPs(dt.parse(file))
    scores = bone_strength(genome)

    print("Bone-strength estimated from genome %s" % file)
    print("(Higher points is stronger bone)\n")

    for title, (max_score, score) in scores.items():
      print("  %18s: %2d of %-2d" % (title, score, max_score))

    max_total = sum(p for p,_ in scores.values())
    total = sum(p for _,p in scores.values())
    print("  %18s: %2d of %2d\n" % ("Total score", total, max_total))

    if not genome.has("rs2908004"):
      print("NOTE: rs2908004 wasn't in this genome; scores may be a bit too high")

if __name__ == "__main__":
  try:
    sys.exit(main(sys.argv[1:]))
  except Exception, e:
    print("Error: %s" % e.message)

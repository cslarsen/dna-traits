# Copyright (C) 2014 Christian Stigen Larsen
# Distributed under the GPL v3 or later. See COPYING.

import unittest

class TestDNA(unittest.TestCase):
  def setUp(self):
    self.failfast = True
    self.genome_file = "../genome.txt"

  def test_import_module(self):
    """Import module dna_traits."""
    try:
      import dna_traits
    except Exception, e:
      self.fail("Could not import dna_traits: %s" % e)

  def test_open_nonexisting_file(self):
    """Open non-existant genome file."""
    import dna_traits
    self.assertRaises(RuntimeError, dna_traits.parse, "")

  def test_parse_file(self):
    """Parse a 23andMe genome file."""
    import dna_traits
    snp = dna_traits.parse(self.genome_file)

    self.assertIsNotNone(snp)
    self.assertGreater(len(snp), 0)

    # RSID
    rsid, genotype = snp.items()[0]
    self.assertEqual(rsid[:2], "rs")
    for digit in rsid[2:]:
      self.assertIn(digit, list("0123456789"))

    # GENOTYPE
    self.assertEqual(len(genotype), 2)
    self.assertIn(genotype[0], list("ATCG-"))
    self.assertIn(genotype[1], list("ATCG-"))

    # NOTE: These SNPs may not be present in all genomes!
    self.assertIn("rs7495174", snp)
    self.assertIn("rs1805007", snp)
    self.assertIn("rs1800401", snp)

if __name__ == "__main__":
  unittest.main(failfast=True)

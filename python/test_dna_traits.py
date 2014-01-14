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
    dna = dna_traits.parse(self.genome_file)
    self.assertIsNotNone(dna)
    self.assertGreater(len(dna), 0)

  def test_genotypes(self):
    import dna_traits
    dna = dna_traits.parse(self.genome_file)
    genotype = dna["rs7495174"]
    self.assertIsNotNone(genotype)
    self.assertEqual(len(genotype), 2)
    self.assertIn(genotype[0], list("ATCG-"))
    self.assertIn(genotype[1], list("ATCG-"))

    # NOTE: These SNPs may not be present in all genomes!
    self.assertIsNotNone(dna["rs7495174"])
    self.assertIsNotNone(dna["rs1805007"])
    self.assertIsNotNone(dna["rs1800401"])

if __name__ == "__main__":
  unittest.main(failfast=True)

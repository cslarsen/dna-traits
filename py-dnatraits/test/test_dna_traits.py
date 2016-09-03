# Copyright (C) 2014, 2016 Christian Stigen Larsen
# Distributed under the GPL v3 or later. See COPYING.

import unittest
import dna_traits as dt

class TestGenome(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.genome = dt.parse("../genomes/genome.txt")

    def test_len(self):
        self.assertGreater(len(self.genome), 0)

    def test_iterator_increases(self):
        rsids = []
        for snp in self.genome:
            self.assertIsInstance(snp.rsid, str)
            rsids.append(int(snp.rsid[2:]))
            if len(rsids)>10:
                break
        self.assertEqual(sorted(rsids), rsids)

    def test_slice(self):
        self.assertEqual(len(self.genome[0:10]), 10)

    def test_last(self):
        self.assertEqual(self.genome.last, 80359889)

    def test_first(self):
        self.assertEqual(self.genome.first, 3)

    def test_y_chromosome(self):
        self.assertIsInstance(self.genome.y_chromosome, bool)
        self.assertIsInstance(self.genome.female, bool)
        self.assertIsInstance(self.genome.male, bool)
        self.assertNotEqual(self.genome.male, self.genome.female)
        self.assertEqual(self.genome.male, self.genome.y_chromosome)

    def test_orientation(self):
        self.assertIsInstance(self.genome.orientation, int)
        self.assertIn(self.genome.orientation, [-1,+1])

    def test_repr(self):
        self.assertIsInstance(str(self.genome), str)
        self.assertIsInstance(repr(self.genome), str)
        self.assertTrue(repr(self.genome).startswith("<Genome"))

    def test_snp(self):
        for rsid in [7495174, 1805007, 1800401]:
            if rsid not in self.genome:
                continue

            NN = dt.SNP([], rsid, 1, 0, 0)

            # Various ways of accessing SNPs
            self.assertIn("rs%d" % rsid, self.genome)
            self.assertIn(rsid, self.genome)
            #
            self.assertIsNotNone(self.genome.__getattr__("rs%d" % rsid))
            self.assertIsNotNone(self.genome["rs%d" % rsid])
            self.assertIsNotNone(self.genome[rsid])
            self.assertIsNotNone(self.genome["rs%d" % rsid])
            self.assertIsNotNone(self.genome[rsid])
            #
            self.assertEqual(self.genome["rs%d" % (rsid*2)], NN)
            self.assertEqual(self.genome[rsid*100], NN)

            snp = self.genome[rsid]
            self.assertIsNotNone(snp)
            self.assertEqual(snp.orientation, self.genome.orientation)
            self.assertEqual(snp.positive(), snp.negative().complement())
            self.assertEqual(snp.negative(), snp.positive().complement())
            if snp.orientation > 0:
                self.assertEqual(~snp, snp.negative())
            else:
                self.assertEqual(~snp, snp.positive())

            for n in list("AGCTDI-"):
                self.assertEqual(snp.count(n), str(snp).count(n))

            # Complement
            self.assertNotEqual(snp.genotype, (~snp).genotype)

            self.assertEqual(snp.rsid, "rs%d" % rsid)
            self.assertEqual(map(dt.Nucleotide, list(str(snp))),
                             snp.genotype)

            self.assertEqual(len(str(snp)), len(snp))
            self.assertGreaterEqual(len(snp), 0)
            self.assertLessEqual(len(snp), 2)
            if len(snp) > 0:
                nuc = dt.Nucleotide(str(snp)[0])
                self.assertEqual(nuc, snp[0])
                self.assertEqual(~nuc, snp[0].complement())
                self.assertEqual(nuc.complement(), ~snp[0])
            if len(snp) > 1:
                nuc = dt.Nucleotide(str(snp)[1])
                self.assertEqual(nuc, snp[1])
                self.assertEqual(~nuc, snp[1].complement())
                self.assertEqual(nuc.complement(), ~snp[1])

            self.assertIsInstance(str(snp), str)
            self.assertIsInstance(repr(snp), str)
            self.assertTrue(repr(snp).startswith("SNP("))

    def test_genotypes(self):
        snp = self.genome["rs7495174"]
        self.assertIsNotNone(snp)
        self.assertEqual(len(snp), 2)
        self.assertIn(str(snp[0]), list("ATCG-DI"))
        self.assertIn(str(snp[1]), list("ATCG-DI"))

        # NOTE: These SNPs may not be present in all genomes!
        self.assertIsNotNone(self.genome["rs7495174"])
        self.assertIsNotNone(self.genome["rs1805007"])
        self.assertIsNotNone(self.genome["rs1800401"])

if __name__ == "__main__":
    unittest.main(failfast=True, verbosity=2)

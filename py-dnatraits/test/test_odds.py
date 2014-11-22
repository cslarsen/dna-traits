"""
Sources for numbers:

  (1) http://www.plosgenetics.org/article/fetchObject.action?uri=info%3Adoi%2F10.1371%2Fjournal.pgen.1001039&representation=PDF

"""

import unittest
from dna_traits.odds import *

class TestOdds(unittest.TestCase):
    def test_pooled_or(self):
        """Pooled OR using Mantel-Haenszel."""
        # From table 3, page 6 in (1)
        ors = [0.88, 0.91]
        pvals = [0.0053, 0.13]
        por, pse, pp =  pooled_or(zip(ors, pvals), 1.15)
        self.assertAlmostEqual(por, 0.89, 2)
        self.assertAlmostEqual(pp, 0.00075, 5)

    def test_pooled_or_2(self):
        """Pooled OR using Mantel-Haenszel, take two."""
        # From table 3, page 6 in (1)
        por, pse, pp = pooled_or(zip([0.89, 0.85], [0.00075, 0.022]), 1.15)
        self.assertAlmostEqual(por, 0.88, 2)
        self.assertAlmostEqual(pp, 5.7e-5)

    def test_confidence_interval(self):
        """Confidence interval from OR, P-value and CI."""
        # From table 3, page 6 in (1)
        ci = confidence_interval(0.89, 0.00075, 0.95)

        self.assertEqual(len(ci), 2)

        self.assertAlmostEqual(min(ci), 0.83, 2)
        self.assertAlmostEqual(max(ci), 0.95, 2)

        self.assertEqual(ci[0], min(ci))
        self.assertEqual(ci[1], max(ci))

    def test_statsig(self):
        """Statistical significance of confidence intervals."""
        self.assertTrue(statsig(
            confidence_interval(0.89, 0.00075, 0.95)))

        self.assertTrue(statsig([-3, -2]))
        self.assertTrue(statsig([-2, -1]))
        self.assertTrue(statsig([-1, 0]))
        self.assertTrue(statsig([-1, 0.5]))
        self.assertTrue(statsig([-0.5, 0]))
        self.assertTrue(statsig([-0.5, 0.99]))
        self.assertTrue(statsig([-0.5, 0.5]))

        self.assertFalse(statsig([-10, 10]))
        self.assertFalse(statsig([-0.5, 1.0]))
        self.assertFalse(statsig([1, 2]))
        self.assertFalse(statsig([0.99, 1.01]))

        self.assertTrue(statsig([1.1, 10.6]))
        self.assertTrue(statsig([1.5, 1.6]))
        self.assertTrue(statsig([2, 3]))
        self.assertTrue(statsig([20, 30]))

    def test_z_value(self):
        """Z-value from P-value."""
        self.assertAlmostEqual(z_value(0.5), 0.67449, 5)
        self.assertAlmostEqual(z_value(0.001), 3.29053, 5)

    def test_p_value(self):
        """P-value from Z-value."""
        self.assertAlmostEqual(p_value(0.67449), 0.5, 5)
        self.assertAlmostEqual(p_value(3.29053), 0.001, 5)

    def test_pz_value_roundtrip(self):
        for n in [0.123, 0, 1, 0.5566, 0.999999, 0.0001]:
            self.assertAlmostEqual(z_value(p_value(n)), n)
            self.assertAlmostEqual(p_value(z_value(n)), n)

    def test_standard_error(self):
        """Standard error from OR and P-value."""
        self.assertAlmostEqual(standard_error(0.89, 0.00075), 0.2640466, 7)

    def test_combined_p_chi2(self):
        self.assertAlmostEqual(combined_p_chi2(
            [0.0645, 0.0910, 0.1148, 0.1315, 0.1319,
             0.1508, 0.1537, 0.1644, 0.1766, 0.1940]), 0.0265, 4)

        self.assertAlmostEqual(combined_p_chi2(
            [0.0412, 0.0523, 0.0911, 0.1011, 0.1267, 0.4500]),
            0.0333, 4)

        self.assertAlmostEqual(combined_p_chi2(
            [0.0412, 0.0523, 0.0911, 0.1011, 0.1267]),
            0.0244, 4)

    def test_combined_p_fisher(self):
        self.assertAlmostEqual(combined_p_fisher([0.001, 0.999]), 0.008, 3)


if __name__ == "__main__":
    unittest.main(failfast=False, verbosity=2)

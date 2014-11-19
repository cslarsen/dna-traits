import unittest
import dna_traits.odds as odds

class TestOdds(unittest.TestCase):
    def test_pooled_or(self):
        """Pooled OR using Mantel-Haenszel."""
        # Source of numbers is table table 3 on page 6 of:
        # http://www.plosgenetics.org/article/fetchObject.action?uri=info%3Adoi%2F10.1371%2Fjournal.pgen.1001039&representation=PDF
        ors = [0.88, 0.91]
        pvals = [0.0053, 0.13]
        pooled_or, pooled_se = odds.pooled_or(zip(ors, pvals))
        self.assertAlmostEqual(pooled_or, 0.89, 2)

    def test_confidence_interval(self):
        """Confidence interval from OR, P-value and CI."""
        ci = odds.confidence_interval(0.89, 0.00075, 0.95)

        self.assertEqual(len(ci), 2)

        self.assertAlmostEqual(min(ci), 0.83, 2)
        self.assertAlmostEqual(max(ci), 0.95, 2)

        self.assertEqual(ci[0], min(ci))
        self.assertEqual(ci[1], max(ci))

    def test_statsig(self):
        """Statistical significance of confidence intervals."""
        self.assertTrue(odds.statsig(
            odds.confidence_interval(0.89, 0.00075, 0.95)))

        self.assertTrue(odds.statsig([-3, -2]))
        self.assertTrue(odds.statsig([-2, -1]))
        self.assertTrue(odds.statsig([-1, 0]))
        self.assertTrue(odds.statsig([-1, 0.5]))
        self.assertTrue(odds.statsig([-0.5, 0]))
        self.assertTrue(odds.statsig([-0.5, 0.99]))
        self.assertTrue(odds.statsig([-0.5, 0.5]))

        self.assertFalse(odds.statsig([-10, 10]))
        self.assertFalse(odds.statsig([-0.5, 1.0]))
        self.assertFalse(odds.statsig([1, 2]))
        self.assertFalse(odds.statsig([0.99, 1.01]))

        self.assertTrue(odds.statsig([1.1, 10.6]))
        self.assertTrue(odds.statsig([1.5, 1.6]))
        self.assertTrue(odds.statsig([2, 3]))
        self.assertTrue(odds.statsig([20, 30]))

    def test_z_value(self):
        """Z-value from P-value."""
        self.assertAlmostEqual(odds.z_value(0.5), 0.67449, 5)
        self.assertAlmostEqual(odds.z_value(0.001), 3.29053, 5)

    def test_standard_error(self):
        """Standard error from OR and P-value."""
        self.assertAlmostEqual(
            odds.standard_error(0.89, 0.00075), 0.2640466, 7)


if __name__ == "__main__":
    unittest.main(failfast=True, verbosity=2)

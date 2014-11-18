"""
Contains speculative functions to deal with odds ratios, such as pooled (or
combined) odds ratios and confidence intervals.

Copyright (C) 2014 Christian Stigen Larsen
Distributed under the GPL v3 or later. See COPYING.

Some sources:

  * http://handbook.cochrane.org/chapter_7/7_7_7_2_obtaining_standard_errors_from_confidence_intervals_and.htm
  * http://stats.stackexchange.com/questions/66760/calculate-odds-ratio-confidence-intervals-from-plink-output
  * http://stats.stackexchange.com/questions/9483/how-to-calculate-confidence-intervals-for-pooled-odd-ratios-in-meta-analysis

"""

from scipy.special import ndtri
import math


def z_value(p_value):
    """Computes Z-value from P-value."""
    return abs(ndtri(p_value/2.0))

def wald_stat(p_value):
    """Computes the Wald statistic from a P-value."""
    return z_value(p_value)**2

def standard_error(beta, p_value):
    """Estimates the standard error of an odds ratio given a
    (non-logarithmic) odds ratio and P-value.

    The beta is the natural logarithm of the odds radio.
    """
    return beta / z_value(p_value)

def confidence_interval(odds_ratio, p_value, output_ci):
    """Returns the confidence interval given an odds ratio, P-value and
    confidence interval percent we want to return (e.g. 0.95).
    """
    beta = math.log(odds_ratio)
    se = standard_error(beta, p_value)
    z = z_value(1.0 - output_ci)
    ci_beta = [beta - z*se, beta + z*se]
    ci_or = map(math.exp, ci_beta)
    return ci_or

def statsig(ci):
    """If the confidence interval (ci) does not include 1.0, it is
    statistically significant."""
    return min(ci) > 1.0

def mantel_haenszel_or(ORs, weights):
    """Computes the pooled odds ratio given ratios and weights using the
    Mantel-Haenszel procedure.

    See http://ijphjournal.it/article/viewFile/5765/5509
    """
    OR_mh = sum(map(lambda (o,w): o*w, zip(ORs, weights))) / sum(weights)
    return OR_mh


def pooled_or(data):
    """Computes the pooled odds ratios by estimating the standard errors
    from the P-values and using them to calculate weights for the
    Mantel-Haenszel procedure.

    Arguments:
        data: List of (odds_ratio, p_value) tuples

    Returns:
        (OR_mh, SE_pooled_or)
    """
    stderrs = [standard_error(or_, pval) for (or_, pval) in data]
    weights = [1.0/se**2 for se in stderrs]
    ors = [or_ for (or_, _) in data]
    or_mh = mantel_haenszel_or(ors, weights)
    se_fem = math.sqrt(1.0 / sum(weights)) # pooled standard error

    # Can use the return values and log(OR_fem) and z = z_value(1.0 - ci) to
    # calculate a confidence interval such as [log(OR_fem) +/- z*se_fem]
    return (or_mh, se_fem)

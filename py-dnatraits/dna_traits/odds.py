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

from scipy.special import ndtri, ndtr
import math


def z_value(p_value):
    """Computes Z-value from P-value."""
    return abs(ndtri(p_value/2.0))

def p_value(z_value):
    return (1.0 - ndtr(z_value))*2.0

def wald_stat(p_value):
    """Computes the Wald statistic from a P-value."""
    return z_value(p_value)**2

def standard_error(beta, p_value):
    """Estimates the standard error of an odds ratio given OR and
    P-value."""
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
    return [min(ci_or), max(ci_or)]

def statsig(ci):
    """If the confidence interval (ci) does not include 1.0, it is
    statistically significant."""
    return not (min(ci) <= 1.0 <= max(ci))

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
        (pooled odds ratio, pooled standard error)
    """
    stderrs = [standard_error(math.log(or_), pval) for (or_, pval) in data]
    weights = [1.0/se**2 for se in stderrs]
    ors = [or_ for (or_, _) in data]
    or_mh = mantel_haenszel_or(ors, weights)
    se_fem = math.sqrt(1.0 / sum(weights)) # pooled standard error

    # Calculate pooled P-value as well (FIXME: this isn't entirely correct!)
    pooled_p = p_value(abs(math.log(or_mh)/se_fem))

    return (or_mh, se_fem, pooled_p)

def relative_risk(odds_ratio, absolute_risk):
    """Converts odds ratio and absolute risk to relative risk.

    The absolute risk is for the unexposed group (the control group).

    See http://en.wikipedia.org/wiki/Odds_ratio
    """
    return odds_ratio / (1.0 - absolute_risk + absolute_risk*odds_ratio)

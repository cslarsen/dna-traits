#!/usr/bin/env python

"""
Shows some health related reports.

Use with caution, this code may contain errors!

Copyright (C) 2014 Christian Stigen Larsen
Distributed under the GPL v3 or later. See COPYING.
"""

import sys

from dna_traits.conditions import inherited_conditions
from dna_traits.health import health_report
from dna_traits.traits import traits_report
import dna_traits as dt

def print_report(title, report, underline="="):
    """Prints a dict with some text formatting."""
    if title is not None:
        print(title)
        if underline is not None:
            print(underline*len(title))

    width = max(map(len, report.keys()))
    for key, value in sorted(report.items()):
        lines = value.split("\n")
        if lines == 1:
            print("%-*s %s" % (width+1, "%s:" % key, value))
        else:
            print("%-*s %s" % (width+1, "%s:" % key, lines[0]))
            for line in lines[1:]:
                print("%-*s %s" % (width+1, " "*(len(key)+1), line))

    if title is not None:
        print("")

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Usage: health-risks genome(s)")
        sys.exit(1)

    for filename in sys.argv[1:]:
        print("%s\n" % filename)
        genome = dt.parse(filename)
        print_report("Health report", health_report(genome))
        print_report("Inherited conditions", inherited_conditions(genome))
        print_report("Traits", traits_report(genome))

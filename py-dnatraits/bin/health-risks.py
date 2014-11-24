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

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Usage: health-risks genome(s)")
        sys.exit(1)

    for filename in sys.argv[1:]:
        genome = dt.parse(filename)
        health = health_report(genome)
        conditions = inherited_conditions(genome)
        traits = traits_report(genome)

        print(filename)

        print("\nHealth risks:")
        for title, description in sorted(health.items()):
            print("%s: %s" % (title, description))

        print("\nInherited conditions:")
        for title, description in sorted(conditions.items()):
            print("%s: %s" % (title, description))

        print("\nTraits:")
        for title, description in sorted(traits.items()):
            print("%s: %s" % (title, description))

#!/usr/bin/env python

"""
Shows some health related reports.

Use with caution, this code may contain errors!

Copyright (C) 2014 Christian Stigen Larsen
Distributed under the GPL v3 or later. See COPYING.
"""

import sys

from dna_traits.health import health_report
import dna_traits as dt

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Usage: health-risks genome(s)")
        sys.exit(1)

    for filename in sys.argv[1:]:
        genome = dt.parse(filename)
        result = health_report(genome)

        print(filename)

        for title, description in sorted(result.items()):
            print("%s: %s" % (title, description))

        print("")

"""Measures parsing speed of dna traits."""

import sys
import timeit

count = 70
print("Measuring parsing speed (%d times) " % count)

sys.stdout.write("[%s]\r[" % ("."*count))
sys.stdout.flush()

best = min(timeit.Timer(
  """
  dt.parse('../genome.txt')
  sys.stdout.write("#");
  sys.stdout.flush()
  """,
  setup = "import dna_traits as dt; import sys")
  .repeat(count, 1))

print("\nThe best speed was {}".format(best))
print("We report only the best time to mitigate OS preemption noise.")

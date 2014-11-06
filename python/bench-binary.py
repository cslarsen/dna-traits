"""Measures parsing speed of dna traits."""

import sys
import timeit

count = 70
print("Measuring parsing speed for binary image (%d times) " % count)

sys.stdout.write("[%s]\r[" % ("."*count))
sys.stdout.flush()

# create binary image
import dna_traits as dt
g = dt.parse("../genome.txt")
g.save("genome.bin")
del g

best = min(timeit.Timer(
  """
  dt.load('genome.bin')
  sys.stdout.write("#");
  sys.stdout.flush()
  """,
  setup = "import dna_traits as dt; import sys")
  .repeat(count, 1))

print("\nThe best speed was {}".format(best))
print("We report only the best time to mitigate OS preemption noise.")

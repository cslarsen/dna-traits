"""Measures parsing speed of dna traits."""

import sys
import timeit

# create binary image
import dna_traits as dt
name = "../genome.txt"
out = "../genome.bin"
#
sys.stdout.write("Parsing %s\n" % name)
sys.stdout.flush()
g = dt.parse(name)
#
sys.stdout.write("Saving %s\n" % out)
sys.stdout.flush()
g.save(out)

count = 20 # FIXME: Bugs out on 70, because of a bug!
print("Measuring reading speed for binary image (%d times) " % count)

sys.stdout.write("[%s]\r[" % ("."*count))
sys.stdout.flush()

best = min(timeit.Timer(
  """
  dt.load('%s')
  sys.stdout.write("#");
  sys.stdout.flush()
  """ % out,
  setup = "import dna_traits as dt; import sys")
  .repeat(count, 1))

print("\nThe best speed was {}".format(best))
print("We report only the best time to mitigate OS preemption noise.")

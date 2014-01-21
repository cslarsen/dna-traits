"""
Script that illustrates how to encode all nucleotide pairs using only 4 bits.

Copyright (C) 2014 Christian Stigen Larsen
Distributed under the GPL v3 or later. See COPYING.

The conditions are that

    * we ignore the internal ordering (e.g. AT and TA are equivalent), and
    * we allow empty nucleotides in one or both positions.

As you can see in the output, we have an unused enumeration value. We use this
to indicate "SNP not found in genome", and its natural value is all bits set to
zero. (We could conceivably use it for other purposes)

The output is

    No. Bits   Pair
     0  0b0    <Not in genome>
     1  0b1    --
     2  0b10   -A
     3  0b11   -C
     4  0b100  -G
     5  0b101  -T
     6  0b110  AA
     7  0b111  AC
     8  0b1000 AG
     9  0b1001 AT
    10  0b1010 CC
    11  0b1011 CG
    12  0b1100 CT
    13  0b1101 GG
    14  0b1110 GT
    15  0b1111 TT

"""

nucleotides = "AGCT-"

pairs = set()
for n1 in nucleotides:
  for n2 in nucleotides:
    pairs.add("".join(sorted(n1+n2)))

print("Unique combinations of nucleotide pairs:")
print("(Internal ordering is disregarded)\n")

print("No. Bits   Pair")
print(" 0  0b0    <Not in genome>")

for no, pair in enumerate(sorted(pairs), 1):
  print("%2d  %-6s %s" % (no, str(bin(no)), pair))

"""
Script that illustrates how to encode all nucleotide pairs using only 5 bits.

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
 4  0b100  -D
 5  0b101  -G
 6  0b110  -I
 7  0b111  -T
 8  0b1000 AA
 9  0b1001 AC
10  0b1010 AD
11  0b1011 AG
12  0b1100 AI
13  0b1101 AT
14  0b1110 CC
15  0b1111 CD
16  0b10000 CG
17  0b10001 CI
18  0b10010 CT
19  0b10011 DD
20  0b10100 DG
21  0b10101 DI
22  0b10110 DT
23  0b10111 GG
24  0b11000 GI
25  0b11001 GT
26  0b11010 II
27  0b11011 IT
28  0b11100 TT

Update: 23andme also has D (delete) and I (insert).
        We thus need 28 bits to represent all of them uniquely.
"""

import math

nucleotides = "AGCT-DI"

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

bits = int(math.ceil(math.log(len(pairs))/math.log(2)))
print("\nResult: Can represent two base-pairs using %d bits" % bits)

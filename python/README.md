DNA-traits Python API
=====================

This directory contains a Python API for DNA-traits.  It is a Python module
for *extremely* fast parsing of 23andMe genome files.

See the main dna-traits page for more information on the underlying C++
code.

Current status
--------------

  * It's _very_ fast. It parses a 23andMe text file in only 0.18 seconds! (on
    _my_ machine)

  * Supports `__getitem__` so you can do `genome["rs123"]` or `genome[123]` or
    even `genome.rs123`.

  * Does not _yet_ support all expected dictionary methods. E.g, no
    `items()`, `keys()`, `values()`, etc.

Usage
-----

Import the module and parse a 23andMe genome file.

    $ python
    >>> import dna_traits as dt
    >>> genome = dt.parse("../genome.txt")
    >>> genome.male
    True
    >>> genome["rs1800401"]
    'GG'
    >>> len(genome)
    949461

Print some SNPs:

    >>> genome["rs7060463"]
    SNP(genotype=[Nucleotide('T'), Nucleotide('-')], rsid='rs7060463',
    orientation=1)
    >>> str(genome["rs7060463"])
    'T-'
    >>> genome.rs7254116.complement()
    SNP(genotype=[Nucleotide('T'), Nucleotide('C')], rsid='rs7254116',
    orientation=1)

Note that you don't get back which chromosome it's on or its position (yet).

Building
--------

You should first compile the main project, then type

    $ cd python
    $ make -j all

and test it with

    $ make check

which should result in

    $ make check
    python test_dna_traits.py -v
    test_genotypes (__main__.TestGenome) ... ok
    test_iterator_increases (__main__.TestGenome) ... ok
    test_len (__main__.TestGenome) ... ok
    test_orientation (__main__.TestGenome) ... ok
    test_repr (__main__.TestGenome) ... ok
    test_slice (__main__.TestGenome) ... skipped 'Unimplemented'
    test_snp (__main__.TestGenome) ... ok
    test_ychromo (__main__.TestGenome) ... ok

    ----------------------------------------------------------------------
    Ran 8 tests in 0.189s

    OK (skipped=1)

Benchmarking speed
------------------

If you type `make bench` it will run a small benchmark to report on speed.
It parses a file 70 times and prints the best result:

    $ make bench
    python bench.py
    Measuring parsing speed (70 times)
    [######################################################################]
    The best speed was 0.175804138184
    We report only the best time to mitigate OS preemption noise.

As you can see the running time may vary a bit and is usually a bit lower
than the pure C++ version, but not by much.


Example application: Measuring bone strength
--------------------------------------------

You can use `bone-strength.py some-genome.txt` to get an indication of bone
strength from a genome.  This code is based on the 23andMe example Go
application, and is in itself based on a PLOS paper by Zheng, et al.

To use it, simply run it on a 23andMe genome file:

    $ ./bone-strength.py ../genome.txt
    Bone-strength estimated from genome ../genome.txt
    (Higher points is stronger bone)

      Lower forearm risk:  4 of  6
       Cortical strength:  3 of  4
             Forearm BMD:  3 of  4
             Total score: 10 of 14

    NOTE: rs2908004 wasn't in this genome; scores may be a bit too high

As you can see, we couldn't find the SNP rs2908004 in the genome, so
calculations may in fact be a bit too high (I'm not sure if 23andMe usually
reads this SNP or not).

The main routine is implemented as

    def bone_strength(genome):
      """Returns bone strength indicators."""
      return {"Cortical strength": (4, 4 - genome.rs9525638.count("T")
                                         - genome.rs2707466.count("C")),
                    "Forearm BMD": (4, 4 - genome.rs2908004.count("G")
                                         - genome.rs2707466.count("C")),
             "Lower forearm risk": (6, 6 - genome.rs7776725.count("C")
                                         - genome.rs2908004.count("G")
                                         - genome.rs2707466.count("C"))}

Example application: Show some results
--------------------------------------

    $ python profile.py ../genome.txt
    ../genome.txt
    Gender:            Male
    Blue eyes:         Likely, if European
    Skin color:        Likely light-skinned of European descent

Author and license
------------------
Copyright (C) 2014 Christian Stigen Larsen,
http://csl.name

Distributed under the GPL v3 or later.

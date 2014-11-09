DNA-traits Python API
=====================

This directory contains a Python API for parsing and querying 23andMe genome
text files.  As the main page says, the main selling point of this project
is *speed*.

Current status
--------------

  * Does not _yet_ support all expected dictionary methods. E.g, no
    `items()`, `keys()`, `values()`, etc.

  * Iterators are currently flaky and extremely slow. I have a solution for
    this that will make it into the Python API later.

  * The .rs<number> accessor will change later and return a function so you
    can do `genome.rs123("AA")` and get a boolean value.  You'll still have
    the other accessors such as `genome["rs123"]` and `genome[123]`.

Usage
-----

Import the module and parse a 23andMe genome file.

    $ python
    >>> import dna_traits as dt
    >>> genome = dt.parse("genome.txt")
    >>> genome.male
    True
    >>> genome["rs1800401"]
    SNP(genotype=[Nucleotide('G'), Nucleotide('G')], rsid='rs1800401',
    orientation=1, chromosome=15, position=28260053)
    >> str(genome.rs1800401)
    'GG'
    >>> len(genome)
    949461

To find the complement:

    >>> snp = genome["rs1800401"]
    >>> ~snp
    SNP(genotype=[Nucleotide('C'), Nucleotide('C')], rsid='rs1800401',
    orientation=1, chromosome=15, position=28260053)

Properties:

    >>> snp.chromosome
    15
    >>> str(snp)
    'GG'
    >>> snp.count("G")
    2
    >>> snp.position
    28260053A
    >> ~snp == "CC"
    True

Building
--------

From the main directory, just type `make -j python-api`.

To run tests:

    $ make check

which should result in

    ~/devel/dna-traits/python csl$ python test_dna_traits.py
    test_genotypes (__main__.TestGenome) ... ok
    test_iterator_increases (__main__.TestGenome) ... ok
    test_len (__main__.TestGenome) ... ok
    test_orientation (__main__.TestGenome) ... ok
    test_repr (__main__.TestGenome) ... ok
    test_slice (__main__.TestGenome) ... ok
    test_snp (__main__.TestGenome) ... ok
    test_y_chromosome (__main__.TestGenome) ... ok

    ----------------------------------------------------------------------
    Ran 8 tests in 0.168s

    OK


Benchmarking speed
------------------

If you type `make bench` it will run a small benchmark to report on speed.
It parses a file 70 times and prints the best result:

    $ m bench
    python bench.py
    Measuring parsing speed (70 times)
    [######################################################################]
    The best speed was 0.162211179733
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

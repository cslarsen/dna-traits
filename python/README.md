DNA-traits Python API
=====================

This directory contains a Python API for DNA-traits.  It is a Python module
for *extremely* fast parsing of 23andMe genome files.

See the main dna-traits page for more information on the underlying C++
code.

Current status
--------------

  * It's _very_ fast. It parses a 23andMe text file in only 0.3 seconds! (on
    _my_ machine)

  * Supports `__getitem__` so you can do `dna["rs123"]`.

  * Does not _yet_ support all expected dictionary methods. E.g, no
    `items()`, `keys()`, `values()`, etc.

Usage
-----

Import the module and parse a 23andMe genome file.

    $ python
    >>> import dna_traits as dt
    >>> dna = dt.parse("../genome.txt")
    >>> dna.ychromo()
    True
    >>> dna["rs1800401"]
    'GG'
    >>> len(dna)
    949461

Print some items:

    >>> dna["rs7060463"]
    'T-'
    >>> dna["rs7254116"]
    'AG'

As you can see, you only get back the genotypes.  There's no information
about which chromosome or position the SNP is in, because the C++ version
doesn't care about anything other than genotypes.

Building
--------

You should first compile the main project, then type

    make -j dna_traits.so

and test it with

    make check

which should result in

    make check
    python test_dna_traits.py -v
    test_import_module (__main__.TestDNA)
    Import module dna_traits. ... ok
    test_open_nonexisting_file (__main__.TestDNA)
    Open non-existant genome file. ... ok
    test_parse_file (__main__.TestDNA)
    Parse a 23andMe genome file. ... ok

    ----------------------------------------------------------------------
    Ran 3 tests in 2.424s

    OK


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

      Lower forearm risk:  4 of 6
       Cortical strength:  3 of 4
             Forearm BMD:  3 of 4
             Total score: 10 of 14

    NOTE: rs2908004 wasn't in this genome; scores may be a bit too high

As you can see, we couldn't find the SNP rs2908004 in the genome, so
calculations may in fact be a bit too high (I'm not sure if 23andMe usually
reads this SNP or not).


Author and license
------------------
Copyright (C) 2014 Christian Stigen Larsen,
http://csl.name

Distributed under the GPL v3 or later.

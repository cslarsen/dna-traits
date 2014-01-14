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
    The best speed was 0.388225078583

    We report the best time only to mitigate OS preemption noise.

As you can see the running time may vary a bit and is usually a bit lower
than the pure C++ version, but not by much.


Author and license
------------------
Copyright (C) 2014 Christian Stigen Larsen,
http://csl.name

Distributed under the GPL v3 or later.

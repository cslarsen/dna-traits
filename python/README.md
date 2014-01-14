DNA-traits Python API
=====================

This directory contains a Python API for DNA-traits.  It is a Python module
for *extremely* fast parsing of 23andMe genome files.

See the main dna-traits page for more information on the underlying C++
code.

Current status
--------------

  * It's _very_ fast. It parses a 23andMe text file in only 0.3 seconds!

  * Supports `__getitem__` so you can do `dna["rs123"]`.

  * Does not _yet_ support all expected dictionary methods. E.g, no
    `items()`, `keys()`, `values()`, etc.

Usage
-----

Import the module and parse a 23andMe genome file.

    $ python
    Python 2.7.5 (default, Aug 25 2013, 00:04:04)
    [GCC 4.2.1 Compatible Apple LLVM 5.0 (clang-500.0.68)] on darwin
    Type "help", "copyright", "credits" or "license" for more information.
    >>> import dna_traits
    >>> snp = dna_traits.parse("../genome.txt")
    >>> len(snp)
    949461

    $ python
    Python 2.7.5 (default, Aug 25 2013, 00:04:04)
    [GCC 4.2.1 Compatible Apple LLVM 5.0 (clang-500.0.68)] on darwin
    Type "help", "copyright", "credits" or "license" for more information.
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

Author and license
------------------
Copyright (C) 2014 Christian Stigen Larsen,
http://csl.name

Distributed under the GPL v3 or later.

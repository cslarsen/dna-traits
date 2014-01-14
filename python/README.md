DNA-traits Python API
=====================

This directory contains a Python API for DNA-traits.

It is a Python module for extremely fast parsing of 23andMe genome files.
See the main dna-traits page for more information.

Current status
--------------

While I use C++ for fast parsing, I copy the entire hash table into a Python
dictionary.  This is both slow and unnecessary.  In a future version I'll
return an object uses `__getitem__` to query the internal C++ hash table.

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

Print some items:

    >>> snp.items()[0]
    ('rs7060463', 'T-')
    >>> snp.items()[-1]
    ('rs7254116', 'AG')
    >>> snp["rs7254116"]
    'AG'

You can see that we only map to genotypes.  There's no information about
which chromosome or position the SNP is in, because the C++ version doesn't
care about anything other than genotypes.

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

dna-traits
==========

This is a *very* fast 23andMe raw genome parser, whose internals are written in
fine-tuned C++. It reads a genome and lets you lookup SNPs based in their
RSIDs:

    >>> import dna_traits
    >>> genome = dna_traits.parse("your-genome.txt")
    >>> len(genome)
    949904
    >>> genome["rs3"]
    SNP(genotype='CC', rsid='rs3', orientation=1, chromosome=13, position=32446842)

Updates
-------

  * September, 2016: Fixed compilation problems with gcc (had only ever tested
    with clang)

The need for speed
------------------

On my machine, an old 2010 MacBook Pro with an SSD, I can parse a 24 Mb genome
and create a dictionary using the `csv` Python module in 2.5 seconds. Pandas
takes around 2.1 seconds — I've even seen some 23andMe parsers take up to 8
seconds!

*This one* consistently takes a mere *0.15 seconds* for the exact same
operation, and also uses *dramatically* less memory (one byte per nucleotide
pair; or six bytes per SNP, which contains chromosome, position and nucleotide
pair).

A more recent ~2013 Xeon CPU Linux machine with SSD takes only **0.07
seconds**!

While slow parsing may not be a big concern for many types of users, the
existence of a fast open source parser may benefit other projects. And,
admittedly, pushing the envelope is always fun!

The Python API
--------------

The project is split up into two parts: A C++ API, and a Python 2 extension
module based on it. You can create bindings for other languages as well.

Here's an example of the Python API, which parses the file, displays `rs123`
and then prints its complement.

    >>> import dna_traits as dt
    >>> genome = dt.parse("genome.txt")
    >>> genome
    <Genome: SNPs=949905, y_chromosome=True, orientation=1>
    >>> genome.male
    True
    >>> genome["rs123"]
    SNP(genotype='AA', rsid='rs123', orientation=1, chromosome=7, position=24966446)
    >>> snp = genome.rs123
    >>> snp.homozygous
    True
    >>> ~snp
    SNP(genotype='TT', rsid='rs123', orientation=1, chromosome=7, position=24966446)

More information can be found in `py-dnatraits/README.md`

Current issues
--------------

  * Currently lacking PyPi support

  * Dependson GNU Make, but an incomplete CMake configuration is in progress.

  * The Python API is currenty somewhat limited and inconsistent, but still
    very much usable!

  * Doesn't parse 23andMe internal IDs yet.

Building
========

Requirements
------------

  * A C++11 compiler

  * Google sparse hash map

  * Genome files in 23andMe format. Many people have uploaded theirs on the
    net for free use. See for example OpenSNP.  If you're a 23andMe
    customer, you can download your own from them.

  * Python development files, if you want to build the Python module.

GNU Make
--------

If Google dense hash map is located in `/usr/local/include`, build
everything, including the Python API, with:

    $ make -j all CXXFLAGS=-I/usr/local/include

Build the `check` target to check if everything works.

    $ make check

CMake
-----

I'm transitioning to CMake, but it's currently not working properly. See
`build-ninja.sh` and `build-make.sh` to test out the current status.

Example of inferring phenotypes
===============================

SNPedia contains the `gs237` criteria for determining whether a person has
blue eyes. At http://snpedia.com/index.php/Gs237/criteria the rule set says:

    and(
      rs4778241(C;C),
      rs12913832(G;G),
      rs7495174(A;A),
      rs8028689(T;T),
      rs7183877(C;C),
      rs1800401(C;C))

In C++ this would be:

    static bool gs237(const Genome& genome)
    {
      return genome[ 4778241] ==  CC
          && genome[12913832] ==  GG
          && genome[ 7495174] ==  AA
          && genome[ 8028689] ==  TT
          && genome[ 7183877] ==  CC
          && genome[ 1800401] == ~CC;
    }

The only thing to note is each SNP's orientation. 23andMe uses positive
orientation, while SNPedia has varying orientation. That's why we flip the
orientation in the last check for the `rs1800401` SNP 

In Python, this can be done in any number of ways, but one way is to use the
``Genome.match`` function:

    all(genome.match((("rs4778241",  "CC"),
                      ("rs12913832", "GG"),
                      ("rs7495174",  "AA"),
                      ("rs8028689",  "TT"),
                      ("rs7183877",  "CC"),
                      ("rs1800401",  "GG"))))


Example reports
===============

I've implemented some of the 23andMe health reports, traits and conditions. You
can see the content here:

  * Traits,
    https://github.com/cslarsen/dna-traits/blob/master/py-dnatraits/dna_traits/traits.py
  * Health,
    https://github.com/cslarsen/dna-traits/blob/master/py-dnatraits/dna_traits/health.py
  * Conditions,
    https://github.com/cslarsen/dna-traits/blob/master/py-dnatraits/dna_traits/conditions.py

Note that there might very well be errors in the above reports, so use with
care! In particular, the odds ratios are most likely very incorrect.  The best
bet is the traits. These should be correct, as well as some simple health
reports. So, the main goal is for this to be educational. In other words,
explore on your own.

Copyright and license
=====================

Copyright (C) 2014, 2016 Christian Stigen Larsen  
http://csl.name

Distributed under GPL v3 or later. See the file COPYING for the full
license.


Places of interest
------------------

  * SNPedia, http://snpedia.com
  * OpenSNP, http://opensnp.org/genotypes
  * dbSNP, http://www.ncbi.nlm.nih.gov/SNP/

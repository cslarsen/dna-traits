The dna-traits 23andMe parser library
=====================================

This is a *very* fast 23andMe raw genome file parser, letting you lookup SNPs
from RSIDs:

    import dna_traits as dt

    genome = dt.parse("genome.txt")

    gender = "man" if genome.y_chromosome else "woman"
    complexion = "light" if genome.rs1426654 == "AA" else "dark"

    eye_color = dt.unphased_match(genome.rs12913832, {
        "AA": "brown",
        "AG": "brown or green",
        "GG": "blue"})

    print("You are a {gender} with {eye_color} eyes and {complexion} skin.".format(
        gender=gender, eye_color=eye_color, complexion=complexion))

In my case, this little program produces

    You are a man with blue eyes and light skin.

The need for speed
==================

On *my* machine, a 2010-era MBP with SSD, I can parse a 24 Mb file using
Python's `csv` module and create a dictionary in 2.5 seconds. Pandas takes
around 2.1 seconds, and I've seen some parsers take up to 8.

In comparison, `dna_traits` takes only *0.15 seconds*, and uses dramatically
less memory (six bytes per SNP, containing its nucleotide pair, chromosome and
position). A more recent 2013-era, Intel Xeon Linux box does the same **in a
blazing 0.07 seconds**!

While I love Pandas for its power and generality, this library is fast because
it is meticulously specialized:  The finely-tuned C++ backend memory maps the
file and *never* scans backwards — every single byte is touched exactly once.
The SNPs are stored in a memory efficient packed struct and stored in Google's
dense hash map, keyed by its 32-bit RSID.

The Python API
==============

The project is split up into two parts: A C++ API, and a Python 2 module
front-end. It should be straight-forward to create bindings for other languages
as well.

Here are some examples illustrating various facets of the Python API:

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
==============

  * The Python module is not ready for PyPi

  * Dependson GNU Make, but an incomplete CMake configuration is in progress.

  * The Python API is currenty somewhat limited and inconsistent, but still
    very much usable!

  * Doesn't parse 23andMe internal IDs yet.

Building
========

The main build system is GNU Make, but I am slowly transitioning to CMake.

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

License
=======

Copyright (C) 2014, 2016 Christian Stigen Larsen  
https://csl.name

Distributed under GPL v3 or later. See the file COPYING for the full license.
This software makes use of open source software; see LICENSES for details.

Additional disclaimer
=====================

In addition to the GPL v3 license terms, and given that this code deals with
health-related issues, I want to stress that the provided code most likely
contains errors, or invalid genome reports.  Results from this code must be
interpreted as HIGHLY SPECULATIVE and may even be downright INCORRECT. Always
consult a medical expert for guidance.  I take NO RESPONSIBILITY whatsoever for
any consequences of using this code, including but not limited to loss of life,
money, spuses, self-esteem and so on. Use at YOUR OWN RISK.

The indended use is for casual, educational purposes. If this code is used for
research purposes, I would be happy if you should cite me. If so, beware that
the parser code may contain serious errors. I would advise you to check for
updates, and also to double-check results with other parsers.

Places of interest
==================

  * SNPedia, http://snpedia.com
  * OpenSNP, http://opensnp.org/genotypes
  * dbSNP, http://www.ncbi.nlm.nih.gov/SNP/

dna-traits
==========

This is a *very* fast 23andMe genome text file parser whose internals are
written in C++.  The genome itself can then be queried using Python; other
language bindings may follow.

The need for speed
------------------

Regarding speed, a naive parser I wrote in pure Python takes around 2.5
seconds to read a million SNPs --- I've also seen some parsers take up to 8
seconds.

*This one* consistently lands on a mere 0.15 seconds on _my_ machine (UPDATE:
On a newer machine with Xeon CPUs, it takes 0.07 seconds!).  It's
so fast is because it memory maps the file and always scans forward ---
every byte in the file is only ever touched once.  To top it off, I'm using
the Google dense hash map for storing SNPs by RSID, which is extremely fast.

While slow parsing is not a big concern for serious users, the existence of
a fast open source parser may also benefit other projects. And, admittedly,
it's a lot of fun trying to push the envelope!

The Python API
--------------

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

Current status
--------------

  * Only 23andMe files are currently supported

  * Parsing is extremely fast: On *my* machine, it only takes 0.15 seconds
    to fully parse a 23andMe genome text file and build up a hash table in
    memory.  That's around 155 Mb/s out of 200 Mb/s possible on my drive.
    UPDATE: On newer Xeon CPUs, it takes only 0.07 seconds!

    In fact, it's fast enough that I won't bother saving the hash table in a
    binary format, as originally intended (Update: I tried it, and reading a
    binary file is only *9 ms* faster than parsing the text file).

  * Rules and phenotype criteria from SNPedia can be written using the
    Python API.

  * In time, I will extract this repo into a Python PyPi 23andMe parser
    library.

  * The Python API is currenty somewhat limited and inconsistent, but still
    very usable!

  * Doesn't parse internal IDs yet.

  * Has support for quite some health, traits and condition reports. See the
    bottom of this page for more information.

Requirements
------------

  * A C++11 compiler

  * Google sparse hash map

  * Genome files in 23andMe format. Many people have uploaded theirs on the
    net for free use. See for example OpenSNP.  If you're a 23andMe
    customer, you can download your own from them.

  * Python development files, if you want to build the Python module.

Building
--------

If Google dense hash map is located in `/usr/local/include`, build
everything, including the Python API, with:

    $ make -j all CXXFLAGS=-I/usr/local/include

Build the `check` target to check if everything works.

    $ make check

Example of inferring phenotypes
-------------------------------

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
---------------

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
---------------------

Copyright (C) 2014, 2016 Christian Stigen Larsen  
http://csl.name

Distributed under GPL v3 or later. See the file COPYING for the full
license.


Places of interest
------------------

  * SNPedia, http://snpedia.com
  * OpenSNP, http://opensnp.org/genotypes
  * dbSNP, http://www.ncbi.nlm.nih.gov/SNP/

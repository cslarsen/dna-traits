dna-traits
==========

Contains a *very* fast 23andMe file parser (written in C++) exposed through
a Python API for querying the genome.

Current status
--------------

  * Only 23andMe files are currently supported

  * Parsing is extremely fast: On *my* machine, it only takes 0.18 seconds
    to fully parse a 23andMe genome text file and build up a hash table in
    memory.  That's around 140 Mb/s out of 200 Mb/s possible on my drive.

    In fact, it's fast enough that I won't bother saving the hash table in a
    binary format, as originally intended (Update: I tried it, and reading a
    binary file is only *9 ms* faster than parsing the text file).

  * Rules and phenotype criteria from SNPedia can be written using the
    Python API.

  * The parser is so fast that I'm planning on extracting it as a separate
    project and make it available through PyPi.

  * The Python API is currenty somewhat limited and inconsistent, but still
    very usable!

Requirements
------------

  * A C++11 compiler

  * Google sparse hash map

  * A 23andMe genome file. Many people have uploaded theirs on the net for free
    use. See for example OpenSNP.

  * Python development files, if you want to build the Python module.

Building
--------

If Google sparse hash map is located in `/usr/local/include`, build
everything, including the Python API, with:

    $ CXXFLAGS=-I/usr/local/include make -j python-api


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

    static bool gs237(const DNA& dna)
    {
      return dna[ 4778241] ==  CC
          && dna[12913832] ==  GG
          && dna[ 7495174] ==  AA
          && dna[ 8028689] ==  TT
          && dna[ 7183877] ==  CC
          && dna[ 1800401] == ~CC;
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


Python API
----------

There is also a Python API, available in the `python/` subdirectory. To
build it, just type

    make python-api

and test it with a genome by typing

    make python-check

There is more information in the `python/` subdirectory. After building, you
can parse 23andMe files and inspect them using Python:

    $ python
    >>> import dna_traits as dt
    >>> genome = dt.parse("genome.txt")
    >>> genome
    Genome(ychromo=True, orientation=1)
    >>> dna.male
    True
    >>> dna.female
    False

You can query for SNPs in several ways:

    >>> genome["rs1800401"]
    SNP(rsid=rs1800401, genotype=GG, orientation=1)

    >>> genome.rs1800401
    SNP(rsid=rs1800401, genotype=GG, orientation=1)

    >>> genome[1800401]
    SNP(rsid=rs1800401, genotype=GG, orientation=1)

    >>> len(genome)
    949461

For the SNP above,

    >>> snp = genome.rs1800401
    >>> snp.count("G")
    2

You can get the complement by doing any of

    >>> snp.complement()
    SNP(rsid=rs1800401, genotype=CC, orientation=1)

    >> ~snp
    SNP(rsid=rs1800401, genotype=CC, orientation=1)

For more information, type `help(object or class)`.

Copyright and license
---------------------

Copyright (C) 2014 Christian Stigen Larsen
http://csl.name

Distributed under GPL v3 or later. See the file COPYING for the full
license.


Places of interest
------------------

  * SNPedia, http://snpedia.com
  * OpenSNP, http://opensnp.org/genotypes
  * dbSNP, http://www.ncbi.nlm.nih.gov/SNP/

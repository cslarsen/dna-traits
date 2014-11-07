dna-traits
==========

This is a *very* fast 23andMe genome text file parser whose internals are
written in C++.  The genome itself can then be queried using Python; other
language bindings may follow.

The need for speed
------------------

Regarding speed, a naive parser I wrote in pure Python takes around 2.5
seconds to read a million SNPs --- I've also seen parser take up to 8
seconds. 

*This one* consistently lands on a mere 0.16 seconds on my (old) machine.
The reason it's so fast is because it memory maps the file and always scans
forward --- every byte in the file is only ever touched once.  To top it
off, I'm using the Google dense hash map for storing SNPs by RSID, which is
extremely fast.

While slow parsing is not a big concern for serious users, the existence of
a fast open source parser may benefit other projects as well It's also a lot
of fun trying to push the envelope!

The Python API
--------------

Here's an example of the Python API, which parses the file, displays `rs123`
and then prints its complement.

    >>> import dna_traits as dt
    >>> genome = dt.parse("genome.txt")
    >>> genome
    <Genome: SNPs=949904, ychromo=True, orientation=1>
    >>> genome["rs123"]
    SNP(genotype=[Nucleotide('A'), Nucleotide('A')], rsid='rs123',
    orientation=1, chromosome=7, position=24966446)
    >>> str(genome.rs123)
    'AA'
    >>> str(~genome.rs123)
    'TT'

Current status
--------------

  * Only 23andMe files are currently supported

  * Parsing is extremely fast: On *my* machine, it only takes 0.16 seconds
    to fully parse a 23andMe genome text file and build up a hash table in
    memory.  That's around 155 Mb/s out of 200 Mb/s possible on my drive.

    In fact, it's fast enough that I won't bother saving the hash table in a
    binary format, as originally intended (Update: I tried it, and reading a
    binary file is only *9 ms* faster than parsing the text file).

  * Rules and phenotype criteria from SNPedia can be written using the
    Python API.

  * In time, I will extract this repo into a Python PyPi 23andMe parser
    library.

  * The Python API is currenty somewhat limited and inconsistent, but still
    very usable!

Requirements
------------

  * A C++11 compiler

  * Google sparse hash map

  * A 23andMe genome file. Many people have uploaded theirs on the net for
    free use. See for example OpenSNP.

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

See the `python/README.md` file for more information on the Python API.

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

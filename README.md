dna-traits
==========

dna-traits infers various phenotypes from 23andMe genome files by using data
from SNPedia.

In other words, you can download your personal genome from 23andMe and run
it through this program, and it will attempt to tell things like your eye
color, and so on.

This is just a personal hobby project I've started to teach me more about
DNA and bioinformatics.


Current status
--------------

  * Only 23andMe files are currently supported

  * Parsing is extremely fast: It takes only 0.3 seconds (on *my* machine,
    at least) to fully parse a 23andMe text file and build up a hash table
    in memory, or about 80 Mb/s.  In fact, it's fast enough that I won't
    bother saving the hash table in a binary format, as originally intended.

  * Rules and phenotype criteria from SNPedia must be hard-coded in C++.
    Later on, it would be nice to put these in some form of interpreted text
    file.

  * Has a Python API for fast parsing and quick experimentation.

Requirements
------------

  * A C++11 compiler

  * Google sparse hash map

  * A 23andMe genome. Many people have uploaded theirs on the net for free
    use. See for example OpenSNP.

  * Python development files, if you want to build the Python module.


Usage
-----

Build the sources by using `make -j32 dna`, save your 23andMe genome as
`genome.txt` and run

    $ /usr/bin/time -lp ./dna genome.txt

and you should get some output like

    Reading genome.txt ... done
    Read 949461 unique SNPs

    Example SNPs in this genome:

      rs7495174 AA
      rs1805007 CC
      rs1800401 GG

    SUMMARY

      Gender:     Male (has Y-chromosome)
      Blue eyes?  Yes (gs237)
      Skin color: Probably light-skinned, European ancestry (rs1426654)

    real         0.37
    user         0.34
    sys          0.02
      34164736  maximum resident set size
             0  average shared memory size
             0  average unshared data size
             0  average unshared stack size
          8360  page reclaims
             0  page faults
             0  swaps
             0  block input operations
             0  block output operations
             0  messages sent
             0  messages received
             0  signals received
             0  voluntary context switches
             1  involuntary context switches


How to add your own rules
-------------------------

Here is how we determine/guess/approximate if the person in question has
blue eyes:

    static bool gs237(const DNA& dna)
    {
      return dna[ 4778241] ==  CC
          && dna[12913832] ==  GG
          && dna[ 7495174] ==  AA
          && dna[ 8028689] ==  TT
          && dna[ 7183877] ==  CC
          && dna[ 1800401] == ~CC;
    }


If you look up the corresponding `gs237` criteria on SNPedia -- at
http://snpedia.com/index.php/Gs237/criteria -- you can see that the code is
almost completely the same as they state:

    and(
      rs4778241(C;C),
      rs12913832(G;G),
      rs7495174(A;A),
      rs8028689(T;T),
      rs7183877(C;C),
      rs1800401(C;C))

The only thing to note is each SNP's orientation. 23andMe uses positive
orientation, while SNPedia has varying orientation. That's why we flip the
orientation in the last check for the `rs1800401` SNP 

You can make your own rules like this. (Later on, I should put the rules in
a text file.)


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
    >>> dna = dt.parse("genome.txt")
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

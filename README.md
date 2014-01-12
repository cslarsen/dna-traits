dna-traits
==========

dna-traits infers various traits from a human genome by using data available
from SNPedia.

This is very much a personal hobby-project to teach me more about DNA and
bioinformatics.

Current status
--------------

Currently only text files from 23andme are supported.

You can run it on a genome and it will report if this person has blue eyes
and the person's gender. Not very interesting at the moment, but still this
is enough code to be able to report on almost everything SNPedia has to
offer (medical conditions, traits, etc.).

The code *is* quite messy, but at least the parsing is now extremely fast.
I use memory mapping and C-style parsing with packed structs to get things
going fast. On my computer, it takes half a second to parse a 23andme text
file and store all SNPs in memory. It works out to about 50 MB/s, but I
think it would actually be higher if the files were bigger (not tested).

Anyway, the old code used 4-5 secs to parse, and the Python version uses 2-3
secs, so 0.5 secs and only two involuntary context switches should be quite
good.

Usage
-----

You need a C++11 compiler.

Build the sources by using `make -j32 dna`, save your 23andme genome as
`genome.txt` and run

    ./dna genome.txt

and you should get some output like

    Reading genome.txt ... done
    Read 949461 unique SNPs

    Example SNPs in this genome:

      rs7495174 AA
      rs1805007 CC
      rs1800401 GG

    SUMMARY

      Gender:     Male (has Y-chromosome)
      Blue eyes?  Yes (based on criteria gs237)
      Skin color: Probably light-skinned, European ancestry (based on rs1426654)

    real         0.57
    user         0.52
    sys          0.04
      64167936  maximum resident set size
             0  average shared memory size
             0  average unshared data size
             0  average unshared stack size
         15677  page reclaims
             7  page faults
             0  swaps
             0  block input operations
             0  block output operations
             0  messages sent
             0  messages received
             0  signals received
             1  voluntary context switches
             8  involuntary context switches

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

The only thing to note is each SNP's orientation. 23andme uses positive
orientation, while SNPedia has varying orientation. That's why we flip the
orientation in the last check for the `rs1800401` SNP 

You can make your own rules like this. (Later on, I should put the rules in
a text file.)

License
-------

GPL v3 or later.

Author
------

Christian Stigen Larsen, http://csl.name

Places of interest
------------------

  * SNPedia, http://snpedia.com
  * OpenSNP, http://opensnp.org/genotypes
  * dbSNP, http://www.ncbi.nlm.nih.gov/SNP/

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

Other than that, the code is a bit messy, the parsing is extremely slow (as
slow as the Python prototype), etc.

UPDATE: The parsing is now very fast. An entire 23andme file is parsed 5-10x
faster than the old code (which is on par with Python). It takes 0.5 secs
and two context switches to parse the file. The code is in read.cpp, though,
and not integrated into the main code yet (`make read` to test it).

Usage
-----

Build the sources by using `make -j32 dna`, save your 23andme genome as
`genome.txt` and run

    ./dna genome.txt

and you should get some output like

    Reading genome.txt ... done

    Read 960613 SNPs

    Example SNPs:
      {rs7495174 genotype=AA chromosome=1 position=28344238}
      {rs1805007 genotype=CC chromosome=1 position=89986117}
      {rs1800401 genotype=GG chromosome=1 position=28260053}

    Summary of findings:
      Gender (has Y-chromosome): Male
      Blue eyes (criteria gs237): Yes

    real         4.70
    user         4.57
    sys          0.11
      73789440  maximum resident set size
             0  average shared memory size
             0  average unshared data size
             0  average unshared stack size
         18023  page reclaims
             9  page faults
             0  swaps
             0  block input operations
             0  block output operations
             0  messages sent
             0  messages received
             0  signals received
             1  voluntary context switches
           546  involuntary context switches

How to add your own rules
-------------------------

Here is how we determine/guess/approximate if the person in question has
blue eyes:

    static bool gs237(const DNA& dna)
    {
      return dna["rs4778241"]  ==  CC
          && dna["rs12913832"] ==  GG
          && dna["rs7495174"]  ==  AA
          && dna["rs8028689"]  ==  TT
          && dna["rs7183877"]  ==  CC
          && dna["rs1800401"]  == ~CC;
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

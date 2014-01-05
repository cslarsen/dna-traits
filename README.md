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

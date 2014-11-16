TARGETS := dna_traits/_dna_traits.so

all: $(TARGETS)

dna_traits/_dna_traits.so: src/_dna_traits.so
	cp -f $^ $@

src/_dna_traits.so:
	$(MAKE) -C src _dna_traits.so

check: all
	python test/test_dna_traits.py

bench: all
	python test/bench.py

clean:
	$(MAKE) -C src clean
	rm -f $(TARGETS)

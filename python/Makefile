# You need to build the main libdnatraits.so first

TARGETS := dna_traits.o genome.o _dna_traits.so libdnatraits.so
CXX := g++
CC := $(CXX)

override CXXFLAGS += -I../include --std=c++11 -Wall \
					 -Ofast -march=native -mtune=native \
					 -flto

PYCFLAGS := $(shell python-config --cflags)
PYLDFLAGS := $(shell python-config --ldflags)

all: $(TARGETS)

dna_traits.o: dna_traits.cpp genome.h
	$(CXX) $(CXXFLAGS) $(PYCFLAGS) -c -o $@ $<

libdnatraits.so: ../libdnatraits.so
	cp $^ $@

genome.o: genome.cpp genome.h
	$(CXX) $(CXXFLAGS) $(PYCFLAGS) -c -o $@ $<

_dna_traits.so: libdnatraits.so dna_traits.o genome.o
	$(CXX) $(PYLDFLAGS) -L. -ldnatraits -shared -o $@ dna_traits.o genome.o

check: _dna_traits.so
	python test_dna_traits.py -v

bench: all
	python bench.py

clean:
	rm -f $(TARGETS) *.pyc

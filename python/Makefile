TARGETS := dna_traits.o genome.o _dna_traits.so
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

genome.o: genome.cpp genome.h
	$(CXX) $(CXXFLAGS) $(PYCFLAGS) -c -o $@ $<

_dna_traits.so: dna_traits.o genome.o ../src/dna.o ../src/parse_file.o ../src/filesize.o
	$(CXX) $(PYLDFLAGS) -shared -o $@ $^

check: _dna_traits.so
	python test_dna_traits.py -v

bench: all
	python bench.py

clean:
	rm -f $(TARGETS) *.pyc

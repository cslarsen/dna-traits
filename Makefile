CXX := g++
CC := $(CXX)

override CXXFLAGS += -Iinclude --std=c++11 -Wall \
					 -Ofast -march=native -mtune=native \
					 -flto

TARGETS := src/summary.o \
					src/dna.o \
					src/parse_file.o \
					src/filesize.o \
					test/test1.o \
					test/test1

PYCFLAGS := $(shell python-config --cflags)
PYLDFLAGS := $(shell python-config --ldflags)

run: test/test1
	/usr/bin/time -lp test/test1 genome.txt

test/test1: src/dna.o src/filesize.o src/parse_file.o src/summary.o test/test1.o

all: $(TARGETS)

python-api: $(TARGETS)
	$(MAKE) -C python _dna_traits.so

python-check: python-api
	$(MAKE) -C python check

python-bench: python-api
	$(MAKE) -C python bench

repl: python-api
	python/repl genome.txt

clean:
	rm -f $(TARGETS)
	cd python; make clean

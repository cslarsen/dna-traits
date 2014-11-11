CXX := g++
CC := $(CXX)

override CXXFLAGS += -Iinclude --std=c++11 -W -Wall \
					 -Ofast -march=native -mtune=native \
					 -flto

OBJFILES := \
	src/dnatraits.o \
	src/file.o \
	src/fileptr.o \
	src/filesize.o \
	src/mmap.o \
	src/parse_file.o

TARGETS := $(OBJFILES) \
	libdnatraits.so \
	test/test1.o \
	test/test1

PYCFLAGS := $(shell python-config --cflags)
PYLDFLAGS := $(shell python-config --ldflags)

run: test/test1
	/usr/bin/time -lp test/test1 genome.txt

test/test1: libdnatraits.so test/test1.o
	$(CXX) $(CXXFLAGS) test/test1.o -o $@ -L. -ldnatraits

libdnatraits.so: $(OBJFILES)
	$(CXX) $(CXXFLAGS) -shared $^ -o $@

all: $(TARGETS)

python-api: libdnatraits.so
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

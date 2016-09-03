CXX := g++
CC := $(CXX)

TARGETS := \
	dna_traits.o \
	genome.o \
	_dna_traits.so \

PYCFLAGS := $(shell python-config --cflags)
PYLDFLAGS := $(shell python-config --ldflags)
PYINCLUDE := $(shell python-config --include)

LD_LIB_PATH := \
	LD_LIBRARY_PATH=$$LD_LIBRARY_PATH:../../dnatraits \
	DYLD_LIBRARY_PATH=$$DYLD_LIBRARY_PATH:../../dnatraits \

override CXXFLAGS += \
	$(PYINCLUDE) \
	-I../../dnatraits/include \
	--std=c++11 \
	-W -Wall \
	-Ofast -march=native -mtune=native -DNDEBUG

all: $(TARGETS)

_dna_traits.so: dna_traits.o genome.o ../../dnatraits/src/libdnatraits.o
	$(CXX) $(PYLDFLAGS) $(CXXFLAGS) -shared -fPIC \
		-o $@ $^

check: _dna_traits.so
	$(LD_LIB_PATH) python test_dna_traits.py -v

bench: _dna_traits.so
	$(LD_LIB_PATH) python bench.py

clean:
	rm -f *.pyc *.pyo $(TARGETS)

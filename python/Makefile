TARGETS = dna_traits.o dna_traits.so

CXX = g++
CC = $(CXX)

CXXFLAGS = -I../include --std=c++11 -Wall \
					 -Ofast -march=native -mtune=native \
					 -flto

PYCFLAGS = $(shell python-config --cflags)
PYLDFLAGS = $(shell python-config --ldflags)

all:
	@echo
	@echo "First build all the files in the parent directory, then you can build"
	@echo "dna_traits.so and test it with \`make check\`"
	@echo

dna_traits.o: dna_traits.cpp
	$(CXX) $(CXXFLAGS) $(PYCFLAGS) -c -o $@ $<

dna_traits.so: dna_traits.o ../src/dna.o ../src/parse_file.o ../src/filesize.o
	$(CXX) $(PYLDFLAGS) -shared -o $@ $^

check: dna_traits.so
	python test_dna_traits.py -v

clean:
	rm -f $(TARGETS) *.pyc

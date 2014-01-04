CXX = /usr/local-gcc/bin/c++
CXXFLAGS = --std=c++11 -Wall -O4 -march=native -mtune=native -fomit-frame-pointer
TARGETS = dna

run: dna
	/usr/bin/time -lp ./dna genome.txt

all: $(TARGETS)

clean:
	rm -f $(TARGETS)

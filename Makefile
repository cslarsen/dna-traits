CXXPATH = /usr/bin
CXX = $(CXXPATH)/c++
CC = $(CXXPATH)/c++
CXXFLAGS = -Iinclude --std=c++11 -Wall \
					 -O4 -march=native -mtune=native \
					 -fomit-frame-pointer
TARGETS = src/summary.o src/dna.o src/parse_file.o dna.o dna

run: dna
	/usr/bin/time -lp ./dna genome.txt

dna: src/dna.o dna.o src/parse_file.o src/summary.o

all: $(TARGETS)

# just testing really fast parsing
read: src/filesize.cpp read.cpp
	g++ -std=c++11 -Wall \
		-O3 \
		-fomit-frame-pointer \
		-march=corei7 -mtune=corei7 \
		$^ -o $@ \
		-Iinclude
	/usr/bin/time -lp ./read genome.txt

clean:
	rm -f $(TARGETS)

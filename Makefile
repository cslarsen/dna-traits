CXX = g++
CC = $(CXX)
CXXFLAGS = -Iinclude --std=c++11 -Wall \
					 -O3 -march=native -mtune=native \
					 -fomit-frame-pointer
TARGETS = src/summary.o \
					src/dna.o \
					src/parse_file.o \
					src/filesize.o \
					dna.o \
					dna

run: dna
	/usr/bin/time -lp ./dna genome.txt

dna: src/dna.o src/filesize.o src/parse_file.o src/summary.o dna.o

all: $(TARGETS)

clean:
	rm -f $(TARGETS)

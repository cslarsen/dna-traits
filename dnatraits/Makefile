CXX := g++
CC := $(CXX)

override CXXFLAGS += \
	-Iinclude \
	-Isrc \
	--std=c++11 \
	-W -Wall \
	-Ofast -march=native -DNDEBUG

override LDFLAGS += \
	-arch x86_64

OBJFILES := \
	src/dnatraits.o \
	src/file.o \
	src/fileptr.o \
	src/filesize.o \
	src/mmap.o \
	src/parse_file.o \

TARGETS := $(OBJFILES) \
	test/test1.o \
	src/libdnatraits.o

all: $(TARGETS)

src/libdnatraits.o: $(OBJFILES)
	$(LD) $(LDFLAGS) -r $^ -o $@

libdnatraits.so: $(OBJFILES)
	$(CXX) $(CXXFLAGS) -shared $^ -o $@

test/test1: $(TARGETS) libdnatraits.so
	$(CXX) $(LDFLAGS) $(CXXFLAGS) -L. -ldnatraits test/test1.o -o $@

check: test/test1
	test/test1 ../genomes/genome.txt

clean:
	rm -f $(TARGETS)

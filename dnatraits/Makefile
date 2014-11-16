CXX := g++
CC := $(CXX)

override CXXFLAGS += \
	-Iinclude \
	-Isrc \
	--std=c++11 \
	-W -Wall \
	-Ofast -march=native -mtune=native -flto

OBJFILES := \
	src/dnatraits.o \
	src/file.o \
	src/fileptr.o \
	src/filesize.o \
	src/mmap.o \
	src/parse_file.o

TARGETS := $(OBJFILES) \
	libdnatraits.so

all: $(TARGETS)

libdnatraits.so: $(OBJFILES)
	$(CXX) $(CXXFLAGS) -shared $^ -o $@

clean:
	rm -f $(TARGETS)

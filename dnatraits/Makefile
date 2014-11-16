CXX := g++
CC := $(CXX)

override CXXFLAGS += \
	-Iinclude \
	-Isrc \
	--std=c++11 \
	-W -Wall \
	-Ofast -march=native -mtune=native -flto

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
	src/libdnatraits.o \
	libdnatraits.so

all: $(TARGETS)

src/libdnatraits.o: $(OBJFILES)
	$(LD) $(LDFLAGS) -r $^ -o $@

libdnatraits.so: $(OBJFILES)
	$(CXX) $(CXXFLAGS) -shared $^ -o $@

check: $(TARGETS)

clean:
	rm -f $(TARGETS)

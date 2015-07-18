# makefile

# options
CC := clang++
FLAGS := -Wall -pedantic -g -pthread -ftemplate-backtrace-limit=1

# globs
SRCS := $(wildcard *.cpp)
HDRS := $(wildcard *.hpp)
OBJS := $(patsubst %.cpp,%.o,$(SRCS))
OBJS += parser.o

# build the interpter executable and aslp the library
all: tetra libtetra.a
	@echo "All Done :)"

# build the tetra executable
tetra: $(OBJS)
	$(CC) $(FLAGS) $(OBJS) -o tetra
	mkdir -p ../../bin
	cp tetra ../../bin/

# build the static library
libtetra.a: $(OBJS)
	ar rcs libtetra.a $(OBJS)
	mkdir -p ../../lib
	cp libtetra.a ../../lib/

# compile an object based on source and headers
%.o: %.cpp $(HDRS) parser.genhpp makefile
	$(CC) $(FLAGS) -c $< -o $@

# build the parser object based on bison generated files
parser.o: parser.gencpp $(HDRS) parser.genhpp makefile
	$(CC) $(FLAGS) -c -x c++ parser.gencpp -o parser.o

# compile the parser with bison
parser.gencpp parser.genhpp: parser.y $(HDRS) makefile
	bison -d -v parser.y -o parser.gencpp

# tidy up
clean:
	rm -f libtetra.a tetra $(OBJS) parser.genhpp parser.gencpp parser.output

.PHONY: all clean


# makefile

# options
CC = clang++
FLAGS = -Wall -pedantic -g -pthread -ftemplate-backtrace-limit=1
OPTIMIZE_MODE = -O3

# globs
SRCS := $(wildcard *.cpp)
HDRS := $(wildcard *.h)
OBJS := $(patsubst %.cpp,%.o,$(SRCS))

# link it all together
all: $(OBJS) $(HDRS) makefile parser.gen.o
	$(CC) $(FLAGS) $(OBJS) parser.gen.o -o tetra
	mkdir -p ../../bin ../../lib
	cp tetra ../../bin/
	ar rcs libtetra.a $(OBJS) parser.gen.o
	cp libtetra.a ../../lib/

# compile an object based on source and headers
%.o: %.cpp $(HDRS) makefile parser.genh
	$(CC) $(FLAGS) -c $< -o $@

# compile the parser
parser.gen.o: parser.gencpp $(HDRS) makefile
	$(CC) $(FLAGS) -c -x c++ parser.gencpp -o parser.gen.o
parser.gencpp parser.genh: parser.y $(HDRS) makefile
	bison -d -v parser.y -o parser.gencpp

# tidy up
clean:
	rm -f libtetra.a tetra $(OBJS) parser.gen*



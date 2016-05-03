# Makefile

# options
CC := clang++
FLAGS := -W -Wall -pedantic -g -pthread -ftemplate-backtrace-limit=1

# globs
SRCS := $(wildcard *.cpp)
HDRS := $(wildcard *.h)
OBJS := $(patsubst %.cpp,%.o,$(SRCS))
OBJS += parser.o

# build the interpter executable, the library and tags
all: tetra libtetra.a tags
	@echo "All Done :)"

# build the tetra executable
tetra: $(OBJS)
	$(CC) $(FLAGS) $(OBJS) -o tetra
	mkdir -p ../../bin
	cp tetra ../../bin/

# build the static library
libtetra.a: $(OBJS)
	ar rcs libtetra.a $(OBJS)

# compile an object based on source and headers
%.o: %.cpp $(HDRS) parser.genh Makefile
	$(CC) $(FLAGS) -c $< -o $@

# build the parser object based on bison generated files
parser.o: parser.gencpp $(HDRS) parser.genh Makefile
	$(CC) $(FLAGS) -c -x c++ parser.gencpp -o parser.o

# compile the parser with bison
parser.gencpp parser.genh: parser.y $(HDRS) Makefile
	bison -d -v parser.y -o parser.gencpp && mv parser.genhpp parser.genh

# generate tag file
tags: $(SRCS) $(HDRS) parser.y
	ctags $(SRCS) $(HDRS) parser.y

# reformat all code
format:
	clang-format -i -style=Google $(SRCS) $(HDRS)

# tidy up
clean:
	rm -f libtetra.a tetra $(OBJS) parser.genh parser.gencpp parser.output tree.gv tree.png

.PHONY: all clean format


# makefile

# options
CC = clang++
FLAGS = -Wall -pedantic -g -pthread -ftemplate-backtrace-limit=1
DEBUG_MODE = -D USE_OBSERVER
OPTIMIZE_MODE = -O3

# globs
SRCS := $(wildcard *.cpp)
HDRS := $(wildcard *.h)
OBJS := $(patsubst %.c,bin/%.o,$(SRCS))


# link it all together
tetra: $(OBJS) $(HDRS) makefile
	$(CC) $(CFLAGS) $(OBJS) -o tetra
	mkdir -p ../../bin ../../lib
	cp tetra ../../bin/
	ar rcs libtetra.a $(OBJS)
	cp libtetra.a ../../lib/


# compile an object based on source and headers
%.o: %.cpp $(HDRS) makefile
	@mkdir -p bin
	$(CC) $(CFLAGS) -c $< -o $@

# tidy up
clean:
	rm -f $(TARGET) $(OBJS)


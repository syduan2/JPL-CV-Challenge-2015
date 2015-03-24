CC = gcc
CFLAGS = -Wall -O3 -funroll-loops -pedantic
CLIBS= -L.  -lm

build: test_pgm
	:

run:
	# use the Linux compare command for finding the difference of the output.
	# http://stackoverflow.com/questions/5132749/imagemagick-diff-an-image
	:

all: build run

test_pgm: test_main.c pnmio.c error.c
	$(CC) -o test_pgm $(CFLAGS) test_main.c pnmio.c error.c $(CLIBS)

depend:
	makedepend -f Makefile $(SRCS) # not sure what this does, TBH.

# DO NOT DELETE THIS LINE -- make depend depends on it.
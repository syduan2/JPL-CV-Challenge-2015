CC = gcc
CFLAGS = -O3 -funroll-loops -pedantic
CLIBS= -L.  -lm
MAIN_EXE=lander
MAIN_OUT=lander_out
DIFF_OUT=diff_out.png

build: test_pgm dem_reader lander

run: test1

all: build run

test1:
	# use the Linux compare command for finding the difference of the output.
	# http://stackoverflow.com/questions/5132749/imagemagick-diff-an-image
	./$(MAIN_EXE) terrains/terrainS0C0R10_100.pgm terrains/terrainS0C0R10_100_dem.raw $(MAIN_OUT)
	compare -metric AE $(MAIN_OUT) terrains/terrainS0C0R10_100.invHazard.pgm $(DIFF_OUT)
	gnome-open $(DIFF_OUT)

lander: lander.cpp pnmio.c
	g++ -o $(MAIN_EXE) $(CFLAGS) lander.cpp pnmio.c error.c

dem_reader: dem_reader.cpp pnmio.c
	g++ -o dem_reader $(CFLAGS) dem_reader.cpp pnmio.c error.c

test_pgm: test_main.c pnmio.c error.c
	$(CC) -o test_pgm $(CFLAGS) test_main.c pnmio.c error.c $(CLIBS)

depend:
	makedepend -f Makefile $(SRCS) # not sure what this does, TBH.

# DO NOT DELETE THIS LINE -- make depend depends on it.
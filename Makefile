CC = gcc
CPPC = g++
CFLAGS = -O3 -funroll-loops -pedantic
CLIBS= -L.  -lm
MAIN_EXE=lander
MAIN_OUT=lander_out
DIFF_OUT=diff_out.png
PNMIO_F=pnmio.c pnmio.h
ERROR_F=error.c error.h
BRUTEFORCE_F=bruteforce.h bruteforce.cpp
VEC3D_F=vec3d.h vec3d.cpp

all: build run

build: test_pgm dem_reader lander

run: test1

test1: lander
	@ # Execute lander to get the latest output
	./$(MAIN_EXE) terrains/terrainS0C0R10_100.pgm terrains/terrainS0C0R10_100_dem.raw $(MAIN_OUT)

	@ echo "How many pixels are different?"
	@ # compare comes from here:
	@ # http://stackoverflow.com/questions/5132749/imagemagick-diff-an-image
	@ # The AE flag stands for absolute error
	@ compare -metric AE $(MAIN_OUT) terrains/terrainS0C0R10_100.invHazard.pgm $(DIFF_OUT)

	@ # Now, open the image for viewing
	@ eog $(DIFF_OUT) &

test2: lander

lander: lander.cpp $(PNMIO_F) $(ERROR_F) $(BRUTEFORCE_F) $(VEC3D_F)
	$(CPPC) -o $(MAIN_EXE) $(CFLAGS) lander.cpp pnmio.c error.c bruteforce.cpp vec3d.cpp

dem_reader: dem_reader.cpp $(PNMIO_F) $(ERROR_F)
	$(CPPC) -o dem_reader $(CFLAGS) dem_reader.cpp pnmio.c error.c

test_pgm: test_main.c $(PNMIO_F) $(ERROR_F)
	$(CC) -o test_pgm $(CFLAGS) test_main.c pnmio.c error.c $(CLIBS)

depend:
	makedepend -f Makefile $(SRCS) # not sure what this does, TBH.

# DO NOT DELETE THIS LINE -- make depend depends on it.
all:
	gcc -lm -I /usr/include/SDL -g neurons.c -o teste.o -O3 -fopenmp `sdl-config --libs`
clean:
	rm -rf teste.o

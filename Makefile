all:
	g++ -lm -I /usr/include/SDL  Neurons.cpp -o teste.o -O3 `sdl-config --libs`
clean:
	rm -rf Teste.o

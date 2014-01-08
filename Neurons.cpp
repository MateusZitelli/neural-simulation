#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <ctime> 
#include <SDL/SDL.h>
#include <math.h>
#include "Neurons.h"
using namespace std;

int main(int argc, char * argv[]){
	srand(time(0));
	Neuron * neurons_list = (Neuron *) malloc ( NEURONS_X * NEURONS_Y * sizeof ( Neuron ) );
	Neuron * connects[10];
	long int fired[NEURONS_X * NEURONS_Y];
	long int fired_index = 0;
	long int i, j, posx, posy;
	for(i = 0; i < NEURONS_X * NEURONS_Y; i++){

		posx = i % NEURONS_X;
		posy = (i / NEURONS_X + 1) % NEURONS_Y;
		connects[0] = &neurons_list[posx + posy * NEURONS_X];

		posx = i % NEURONS_X;
		posy = (i / NEURONS_X - 1);
		if(posy < 0) posy += NEURONS_X;
		connects[1] = &neurons_list[posx + posy * NEURONS_X];

		posx = (i + 1) % NEURONS_X;
		posy = i / NEURONS_X;
		connects[2] = &neurons_list[posx + posy * NEURONS_X];

		posx = (i - 1) % NEURONS_X;
		posy = i / NEURONS_X;
		if(posx < 0) posx += NEURONS_X;
		connects[3] = &neurons_list[posx + posy * NEURONS_X];
		for(j = 4; j < 10; j ++){
			if(rand() % 100000 / 100000.0 < P){
				connects[j] = &neurons_list[rand() % (NEURONS_X * NEURONS_Y)];
			}else{
				break;
			}
		}
		neurons_list[i].Create(i % NEURONS_X, i / NEURONS_X, &connects[0], j, 2);// rand() % 1000 > 100);
	}
	if (SDL_Init(SDL_INIT_VIDEO) < 0 ) return 1; 
	SDL_Event event;
	SDL_Surface *screen;
	if (!(screen = SDL_SetVideoMode(WIDTH, HEIGHT, DEPTH, SDL_HWSURFACE)))
	{
		SDL_Quit();
		return 1;
	}
	bool keypress = 0;
	int t = 0;
	while(!keypress){
		while(SDL_PollEvent(&event)){
			switch (event.type){
				case SDL_QUIT:
					keypress = 1;
					break;
				case SDL_MOUSEBUTTONDOWN:
					if(event.button.x < NEURONS_X && event.button.y < NEURONS_Y){
						fired[fired_index++] = i;
						neurons_list[event.button.y * NEURONS_X + event.button.x].send();
					}
			}
		}
		for(i = 0; i < NEURONS_X * NEURONS_Y; i++){
			neurons_list[i].run();
			if(neurons_list[i].v >= 1){
				fired[fired_index++] = i;
			}
			//cout << neurons_list[i].v << "\n";
		}
		for(i = 0; i < NEURONS_X * NEURONS_Y; i++){
			//cout << neurons_list[i].v << "\n";
			int color = sqrt(neurons_list[i].v * neurons_list[i].v) * 255;
			setpixel(screen, neurons_list[i].x, neurons_list[i].y, color, color, color);
		}
		setpixel(screen, (int)(t / 3.0) % WIDTH, -fired_index / 100.0 + 1000, 255, 255, 255);
		if((int)(t / 3.0) % WIDTH == 0){
			for(i = 0; i < WIDTH; i ++){
				for(j = 0; j < HEIGHT; j ++){
					setpixel(screen, i, j, 0, 0, 0);
				}
			}
		}
		SDL_Flip(screen);
		for(i = 0; i < fired_index; i++){
			setpixel(screen, neurons_list[fired[i]].x, neurons_list[fired[i]].y, 0, 0, 0);
		}
		for(i = 0; i < NEURONS_X * NEURONS_Y; i++){
			neurons_list[i].update();
		}
		for(i = 0; i < fired_index; i++){
			setpixel(screen, neurons_list[fired[i]].x, neurons_list[fired[i]].y, 255, 255, 255);
			neurons_list[fired[i]].send();
			neurons_list[fired[i]].v = 0;
		}
		fired_index = 0;
		++t;
	}
	SDL_Quit();
	return(0);
}

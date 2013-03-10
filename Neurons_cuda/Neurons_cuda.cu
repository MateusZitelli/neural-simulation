#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <ctime> 
#include <SDL/SDL.h>
#include <math.h>
#define NEURONS_X 100
#define NEURONS_Y 100
#define WIDTH 1000
#define HEIGHT 1000
#define BPP 4
#define DEPTH 32

void setpixel(SDL_Surface *screen, int x, int y, Uint8 r, Uint8 g, Uint8 b)
{
	if(x > WIDTH || y > WIDTH || x < 0 || y < 0) return;
	Uint32 *pixmem32;
	Uint32 colour;  
 
	colour = SDL_MapRGB( screen->format, r, g, b );
  
	pixmem32 = (Uint32*) screen->pixels  + screen->w * y + x;
	*pixmem32 = colour;
}

int main(void){
    long mesh_size = NEURONS_X * NEURONS_Y;
    long i, j;
    bool * type = (bool *) malloc(sizeof(bool) * mesh_size);
    double * a = (double *) malloc(sizeof(double) * mesh_size);
    double * b = (double *) malloc(sizeof(double) * mesh_size);
    double * c = (double *) malloc(sizeof(double) * mesh_size);
    double * d = (double *) malloc(sizeof(double) * mesh_size);
    double * v = (double *) malloc(sizeof(double) * mesh_size);
    double * u = (double *) malloc(sizeof(double) * mesh_size);
    double * I = (double *) malloc(sizeof(double) * mesh_size);
    if (SDL_Init(SDL_INIT_VIDEO) < 0 ) return 1; 
    SDL_Event event;
    SDL_Surface *screen;
    if (!(screen = SDL_SetVideoMode(WIDTH, HEIGHT, DEPTH, SDL_HWSURFACE))){
        SDL_Quit();
        return 1;
    }
    srand(time(0));
    bool keypress = 1;
    for(i = 0; i < mesh_size; i++){
        type[i] = rand() % 2;
        float var = ((float)(rand() % 1000) / 1000);
        if(type[i]){
            a[i] = 0.02;
            b[i] = 0.2;
            c[i] = -65 + 15 * var * var;
            d[i] = 8 - 6 * var * var;
        }else{
            a[i] = 0.02 + 0.08 * var;
            b[i] = 0.25 - 0.05 * var;
            c[i] = -65;
            d[i] = 2;
        }
        v[i] = -65;
        u[i] = b * v;
        I[i] = 0;
    }
    while(keypress){
        while(SDL_PollEvent(&event)){
            switch (event.type){
                case SDL_QUIT:
                    keypress = 1;
                    break;
            }
        }
        for(i = 0; i < mesh_size; i++){
            I[i] = (type[i]?5:2) * ((float)(rand() % 1000) / 1000);
            if(v[i] >= 30){
                setpixel(screen, i % NEURONS_Y , i % NEURONS_X, 255, 255, 255);
                v[i] = c[i];
                u[i] += d[i];
                for(j = 0; j < 8; j++) I[rand() % mesh_size] += 20;
            }
        }
        SDL_Flip(screen);
        for(i = 0; i < mesh_size; i++){
            v[i] += 0.5 * (0.04 * v[i] * v[i] + 5 * v[i] + 150 - u[i] + I[i]);
            v[i] += 0.5 * (0.04 * v[i] * v[i] + 5 * v[i] + 150 - u[i] + I[i]);
            u[i] += a[i] * (b[i] * v[i] - u[i]);
        }
        for(i = 0; i < NEURONS_X; i++){
            for(j = 0; j < NEURONS_Y; j++){
                setpixel(screen, i, j, 0, 0, 0);
            }
        }
    }
}


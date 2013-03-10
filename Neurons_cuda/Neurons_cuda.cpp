#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <ctime>
#include <SDL/SDL.h>
#include <math.h>
#define NEURONS_X 300
#define NEURONS_Y 300
#define WIDTH 1000
#define HEIGHT 1000
#define BPP 4
#define DEPTH 32

using namespace std;

struct connection {
	float *IS[8];
};

void setpixel(SDL_Surface * screen, int x, int y, Uint8 r, Uint8 g, Uint8 b)
{
	if (x > WIDTH || y > WIDTH || x < 0 || y < 0)
		return;
	Uint32 *pixmem32;
	Uint32 colour;

	colour = SDL_MapRGB(screen->format, r, g, b);

	pixmem32 = (Uint32 *) screen->pixels + screen->w * y + x;
	*pixmem32 = colour;
}

int main(void)
{
	long mesh_size = NEURONS_X * NEURONS_Y;
	long i, j, x, y;
	bool *type = (bool *) malloc(sizeof(bool) * mesh_size);
	float *a = (float *)malloc(sizeof(float) * mesh_size);
	float *b = (float *)malloc(sizeof(float) * mesh_size);
	float *c = (float *)malloc(sizeof(float) * mesh_size);
	float *d = (float *)malloc(sizeof(float) * mesh_size);
	float *v = (float *)malloc(sizeof(float) * mesh_size);
	float *u = (float *)malloc(sizeof(float) * mesh_size);
	float *I = (float *)malloc(sizeof(float) * mesh_size);
	float ** C =
	    (float **)malloc(sizeof(float *) *
					mesh_size * 8);
	float *W =
		(float *)malloc(sizeof(float) *
					mesh_size * 8);
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
		return 1;
	SDL_Event event;
	SDL_Surface *screen;
	if (!(screen = SDL_SetVideoMode(WIDTH, HEIGHT, DEPTH, SDL_HWSURFACE))) {
		SDL_Quit();
		return 1;
	}
	srand(time(0));
	bool keypress = 1;
	for (i = 0; i < mesh_size; i++) {
		long px = i % NEURONS_X;
		long py = i / NEURONS_X;
		type[i] = rand() % 1000 > 100;
		float var = ((float)(rand() % 1000) / 1000);
		if (type[i]) {
			a[i] = 0.02;
			b[i] = 0.2;
			c[i] = -65 + 15 * var * var;
			d[i] = 8 - 6 * var * var;
		} else {
			a[i] = 0.02 + 0.08 * var;
			b[i] = 0.25 - 0.05 * var;
			c[i] = -65;
			d[i] = 2;
		}
		v[i] = -65;
		u[i] = b[i] * v[i];
		I[i] = 0;
		int l = 0;
		for (x = -1; x < 2; x++) {
			for (y = -1; y < 2; y++) {
				if (x == 0 && y == 0)
					continue;
				if(type[i])
					W[i * 8 + l++] = 1;
				else
					W[i * 8 + l++] = 1;
				if (px + x > NEURONS_X - 1 || py + y > NEURONS_Y - 1) {
					C[i * 8 + l++] = NULL;
				} else {
					C[i * 8 + l++] =
					    &I[(py + y) * NEURONS_X + px + x];
				}
			}
		}
	}
	long t = 0;
	while (keypress) {
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_QUIT:
				keypress = 0;
				break;
			}
		}
		for (i = 0; i < mesh_size; i++) {
			I[i] =
			    (type[i] ? 5 : 2) * ((float)(rand() % 1000) / 1000);
			if (v[i] >= 30) {
				setpixel(screen, i / NEURONS_X, i % NEURONS_X,
					 255, 255, 255);
				v[i] = c[i];
				u[i] += d[i];
				for (j = 0; j < 8; j++) {
					if (C[i * 8 + j] == NULL)
						continue;
					*(C[i * 8 + j]) +=  30;
				}
			}
		}
		SDL_Flip(screen);
		for (i = 0; i < mesh_size; i++) {
			v[i] +=
			    0.5 * (0.04 * v[i] * v[i] + 5 * v[i] + 150 - u[i] +
				   I[i]);
			v[i] +=
			    0.5 * (0.04 * v[i] * v[i] + 5 * v[i] + 150 - u[i] +
				   I[i]);
			u[i] += a[i] * (b[i] * v[i] - u[i]);
		}
		for (i = 0; i < NEURONS_X + 1; i++) {
			for (j = 0; j < NEURONS_Y + 1; j++) {
				setpixel(screen, j, i, 0, 0, 0);
			}
		}
		t++;
	}
}

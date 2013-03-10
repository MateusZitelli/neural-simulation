#include "SDL/SDL.h"
#define NEURONS_X 300
#define NEURONS_Y 300
#define WIDTH 1000
#define HEIGHT 1000
#define BPP 4
#define DEPTH 32

void setpixel(SDL_Surface *screen, int x, int y, Uint8 r, Uint8 g, Uint8 b)
{
	if(x > WIDTH || y > WIDTH || x < 0 || y < 0) return;
	if(r > 255) r = 255;
	else if(r < 0) r = 0;
	if(g > 255) g = 255;
	else if(g < 0) g = 0;
	if(b > 255) b = 255;
	else if(b < 0) b = 0;
	Uint32 *pixmem32;
	Uint32 colour;  
 
	colour = SDL_MapRGB( screen->format, r, g, b );
  
	pixmem32 = (Uint32*) screen->pixels  + screen->w * y + x;
	*pixmem32 = colour;
}

class Neuron {
	public:
		bool type;
		int x,y;
		float a,b,c,d,v,u,I;
		Neuron * connected[8];
		void Create(int, int, Neuron**, bool t);
		void run();
		void update();
		void send();
};


void Neuron::Create(int vx, int vy, Neuron ** con, bool t){
	int i;
	double var;
	for(i = 0; i < 8; i++){
		connected[i] = con[i];
	}
	type = t;
	x = vx;
	y = vy;
	var = 0;//((float)(rand() % 1000) / 1000);
	if(t){
		a = 0.02;
		b = 0.2;
		c = -65 + 15 * var * var;
		d = 8 - 6 * var * var;
	}else{
		a = 0.02 + 0.08 * var;
		b = 0.25 - 0.05 * var;
		c = -65;
		d = 2;
	}
	v = -64;
	u = b * v;
	I = 0;
}

void Neuron::run(void){
	if(type){
		I = 5;// * ((float)(rand() % 1000) / 1000);
	}else{
		I = 2;// * ((float)(rand() % 1000) / 1000);
	}
}

void Neuron::update(void){
	v += 0.5 * (0.04 * v * v + 5 * v + 150 - u + I);
	v += 0.5 * (0.04 * v * v + 5 * v + 150 - u + I);
	u += a * (b * v - u);
}

void Neuron::send(void){
	int i;
	if(type){
		for(i = 0; i < 8; i++){
			if(connected[i] != NULL)(*connected[i]).I += 10;//* ((float)(rand() % 1000) / 1000);
		}
	}else{
		for(i = 0; i < 8; i++){
			if(connected[i] != NULL)(*connected[i]).I -= 10;//* ((float)(rand() % 1000) / 1000);
		}
	}
}


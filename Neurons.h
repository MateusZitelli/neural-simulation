#include "SDL/SDL.h"
#define NEURONS_X 300
#define NEURONS_Y 300
#define WIDTH 1000
#define HEIGHT 1000
#define BPP 4
#define DEPTH 32
#define P 0.3

using namespace std;

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
		int x,y,t,co,del;
		float a,b,c,d,l,v,u,gsyn;
		Neuron **connected;
		float * inputs;
		void Create(int, int, Neuron**, int, int);
		void run();
		void update();
		void send();
};


void Neuron::Create(int vx, int vy, Neuron * con[10], int connections, int delay){
	int i;
	double var;
	connected = (Neuron **) malloc(connections * sizeof(Neuron *));
	for(i = 0; i < connections; i++){
		connected[i] = con[i];
	}
	co = connections;
	inputs = (float *) malloc(delay * sizeof(float));
	for(i = 0; i < delay; i++){
		inputs[i] = 0;
	}
	x = vx;
	y = vy;
	c = 0; //Ground State
	l = 1;
	v = 1.1 * ((rand() % 10000) / 10000.0);
	del = delay;
	gsyn = 1.0;
}

void Neuron::run(void){
	
}

void Neuron::update(void){
	int i;
	//cout << inputs[(t + del/2) % del] << "\n";
	v += (-v + gsyn * inputs[(t + del/2) % del]) / l 
;	
	inputs[t] = 0;
	t = (t + 1) % del;
}

void Neuron::send(void){
	int i;
	for(i = 0; i < co; i++){
		connected[i]->inputs[connected[i]->t] += 1;
	}
}


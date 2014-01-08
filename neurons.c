#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL.h>

#define X 300
#define Y 300
#define BPP 4
#define DEPTH 32
#define P 0.1 //Probabilidade de conexao - Mundo pequeno

struct pulse_value{
	float value;
	struct pulse_value * next;
};

struct impulse_queue{
	struct pulse_value * head;
	struct pulse_value * tail;
};

struct neuron{
	int x,y, n_sinapses;
	float vi, gsyn, iext, tm, v;
	struct impulse_queue * impulses;
	struct neuron ** sinapses;
};

void setpixel(SDL_Surface *screen, int x, int y, Uint8 r, Uint8 g, Uint8 b)
{
	if(x > X || y > Y + 100 || x < 0 || y < 0) return;
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


struct impulse_queue * add_signal(struct impulse_queue * q, float value){
	struct pulse_value * p = (struct pulse_value *) malloc(sizeof(struct pulse_value));
	p->value = value;
	p->next = NULL;
	if(NULL == q){
		fprintf(stderr, "LINE: %d, queue is not alocated\n", __LINE__);
		free(p);
	}else if(NULL == q->head && NULL == q->tail){
		q->head = q->tail = p;
		return q;
	}else if(NULL == q->head || NULL == q->tail){
		printf("Something wrong with the queue.");
	}else{
		q->tail->next = p;
		q->tail = p;
		return q;
	}
}

int neuro_index(int i, int j){
	return j * Y + i;
}

struct impulse_queue * create_new_queue(int delay){
	int i;
	struct impulse_queue * q = (struct impulse_queue *) malloc(sizeof(struct impulse_queue));
	if(NULL == q){
		fprintf(stderr, "LINE: %d, malloc() failed\n", __LINE__);
	}
	for(i = 0;i < delay - 1; i++){
		add_signal(q, 0);
	}
	return q;
}

float get_signal(struct impulse_queue * q){
	float value = q->head->value;
	struct pulse_value * next = q->head->next;
	free(q->head);
	q->head = next;
	return value;
}

struct neuron * neuron_list(){
	int i,j, k, index, other_index, rand_index;
	struct neuron ** new_sinapses;
	struct neuron * neurons = (struct neuron *) malloc(X * Y * sizeof(struct neuron));
	if(NULL == neurons){
		fprintf(stderr, "LINE: %d, malloc() failed\n", __LINE__);
	}
	for(i = 0; i < X; i++){
		for(j = 0; j < Y; j++){
			index = neuro_index(i,j);
			neurons[index].x = i;
			neurons[index].y = j;
			neurons[index].v = (rand() % 100000) / 100000.0;
			neurons[index].iext = 0.85;
			neurons[index].gsyn = 0.2;
			neurons[index].tm = 10;
			neurons[index].impulses = create_new_queue(10);
			neurons[index].n_sinapses = 4;
			for(k = 0; k< 10; k++){
				if((rand() % 1000000) / 1000000.0 < P){
					neurons[index].n_sinapses += 1;
				}else{
					break;
				}
			}
			neurons[index].sinapses = (struct neuron **) malloc(neurons[index].n_sinapses * sizeof(struct neuron *));
			if(NULL == neurons[index].sinapses){
				fprintf(stderr, "LINE: %d, malloc() failed\n", __LINE__);
			}
			if(i-1 >= 0){
				other_index = neuro_index(i-1,j);
				neurons[index].sinapses[0] = &neurons[other_index];
			}else{
				neurons[index].sinapses[0] = NULL;
			}
			if(i+1 < X){
				other_index = neuro_index(i+1,j);
				neurons[index].sinapses[1] = &neurons[other_index];
			}else{
				neurons[index].sinapses[1] = NULL;
			}
			if(j-1 >= 0){
				other_index = neuro_index(i,j-1);
				neurons[index].sinapses[2] = &neurons[other_index];
			}else{
				neurons[index].sinapses[2] = NULL;
			}
			if(j+1 < Y){
				other_index = neuro_index(i,j+1);
				neurons[index].sinapses[3] = &neurons[other_index];
			}else{
				neurons[index].sinapses[3] = NULL;
			}
			for(k = 4; k< neurons[index].n_sinapses; k++){
				rand_index = rand() % (X * Y);
				neurons[index].sinapses[k] = &neurons[rand_index];
			}

		}
	}
	return neurons;
}

void spike(struct neuron * n, SDL_Surface *screen){
	int i = 0;
	for(i = 0; i < n->n_sinapses; i++){
		if(n->sinapses[i] != NULL){
			n->sinapses[i]->impulses->tail->value += 1;
		}
	}
	setpixel(screen, n->x, n->y, 255, 0, 0);
}

void update_neuron(struct neuron * n){
	float input = get_signal(n->impulses);
	float dv = (-n->v + n->iext + n->gsyn * input + 0.30) / n->tm;
	n->v += dv;
	add_signal(n->impulses, 0);
}

void spike_neuron(struct neuron * n, SDL_Surface *screen){
	if(n->v >= 1){
		n->v = 0;
		spike(n, screen);
	}else{
		setpixel(screen, n->x, n->y, n->v * 100, n->v * 100, n->v * 100);
	}
}

void activity_graphic(SDL_Surface *screen,struct neuron * n, int t){
	int i = 0;
	float sum = 0;
	for(i = 0; i < X * Y; i++){
		sum += n[i].v;
	}
	setpixel(screen, t % X, Y + 100 - sum/(X * Y) * 100, 255, 255, 255);
}

int main(void){
	srand(time(0));
	struct neuron * neurons = neuron_list();
	int i,j,k;
	int t = 0;
	if(NULL == neurons){
		fprintf(stderr, "LINE: %d, malloc() failed\n", __LINE__);
	}

	if (SDL_Init(SDL_INIT_VIDEO) < 0 ) return 1; 
	SDL_Event event;
	SDL_Surface *screen;

	if (!(screen = SDL_SetVideoMode(X, Y + 100, DEPTH, SDL_HWSURFACE)))
	{
		SDL_Quit();
		return 1;
	}

	int keypress = 0;

	while(!keypress){
		while(SDL_PollEvent(&event)){
			switch (event.type){
				case SDL_QUIT:
					keypress = 1;
					break;
				case SDL_MOUSEBUTTONDOWN:
					if(event.button.x < X && event.button.y < Y){
						neurons[ event.button.x - 10 + event.button.y * Y].v = 1;
					}
			}
		}
		#pragma omp parallel for
		for(j = 0; j < X * Y; j++){
			spike_neuron(&neurons[j], screen);
		}
		#pragma omp parallel for
		for(j = 0; j < X * Y; j++){
			update_neuron(&neurons[j]);
		}
		for(j = 0; j < 100; j++){
			for(k = 0; k < 10; k++){
				printf("OIII\n");
			}
		}
		activity_graphic(screen, neurons, t);
		SDL_Flip(screen);
		if(t % X == 0 && t)
			SDL_FillRect(screen,NULL, 0x000000);
		t+= 1; 
	}
	return 0;
}

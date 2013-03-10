#include <cuda.h>
#include "Neurons.h"

__global__ void verify_fired(Neuron *a, long *b, long *i){
  int ID = blockIdx.x;
  if (ID<NEURONS_X * NEURONS_Y){
    if(a[ID].I >= 30) b[i++] = ID;
  }
}

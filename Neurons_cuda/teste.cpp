#include <iostream>
#include <cuda.h>
#define N 100

using namespace std;

__global__ void add(float *a, float *b, float *c){
	int ID=blockIdx.x;
	if(ID < N)
		c[ID] = a[ID] + b[ID];
}

int main(void){
	float a[N], b[N], c[N];
	float *ga, *gb, *gc;
	int i;
	for(i = 0; i < N; i++){
		a[i] = i;
		b[i] = i;
	}
	cudaMalloc((void **) &ga, N * sizeof(float));
	cudaMalloc((void **) &gb, N * sizeof(float));
	cudaMalloc((void **) &gc, N * sizeof(float));

	cudaMemcpy(ga, a, N * sizeof(float), cudaMemcpyHostToDevice);
	cudaMemcpy(gb, b, N * sizeof(float), cudaMemcpyHostToDevice);

	add<<<N,1>>>(ga, gb, gc);
	cudaMemcpy(c, gc, N * sizeof(float), cudaMemcpyDeviceToHost);
	for(i = 0; i < N; i++) cout << c[i] << "\n";
}

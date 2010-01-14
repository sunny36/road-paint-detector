#include "convolution.h"

int Convolution::convolve1D(int* in, int* out, int dataSize, int* kernel, int kernelSize){
    int i, j, k;
    // check validity of params
    if(!in || !out || !kernel) return -1;
    if(dataSize <=0 || kernelSize <= 0) return -1;
    // start convolution from out[kernelSize-1] to out[dataSize-1] (last)
    for(i = kernelSize-1; i < dataSize; ++i){
        out[i] = 0;                             // init to 0 before accumulate
        for(j = i, k = 0; k < kernelSize; --j, ++k){
					out[i] += in[j] * kernel[k];
				}	            
    }
    // convolution from out[0] to out[kernelSize-2]
    for(i = 0; i < kernelSize - 1; ++i){
        out[i] = 0;                             // init to 0 before sum
        for(j = i, k = 0; j >= 0; --j, ++k){
					out[i] += in[j] * kernel[k];
				}            
    }
    return 0;
}

#include "io.h"

void printMatrixFloat(int rows, int cols, CvMat* matrix){
	int i, j;
	for(i = 0; i < rows; ++i){
		for(j =0; j < cols; ++j){
			fprintf(stderr, " %g\t", CV_MAT_ELEM(*matrix, float, i, j));
		}
		printf("\n");
	}
}

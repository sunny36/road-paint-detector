#include "io.h"

void IO::printMatrixFloat(CvMat* matrix, int rows, int cols){
	int i, j;
	for(i = 0; i < rows; ++i){
		for(j =0; j < cols; ++j){
			fprintf(stderr, " %g\t", CV_MAT_ELEM(*matrix, float, i, j));
		}
		printf("\n");
	}
}

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>

#define MAX_VAL 10
#define MATRIX_SIZE 10000

int *matrixgenerate(int dimension){
	int* matrix = (int*)malloc(dimension*dimension*sizeof(int));
	for(int i=0; i<dimension * dimension; i++){
		matrix[i] = rand()%MAX_VAL;
	}
	return matrix;
}

void matrixdisplay(int *matrix, int dimension){
	for(int i=0; i<dimension*dimension; i++){
		printf("%d	",matrix[i]);
		if(((i+1)%dimension)==0)
			printf("\n");
	}
	printf("\n");
}

int* matrixTranspose(int *matrix, int dimension){
	int *mt = (int*)malloc(dimension*dimension*sizeof(int));
	for(int i=0;i<dimension;i++){
		for(int j=0;j<dimension;j++){
			mt[i*dimension+j] = matrix[j*dimension+i];
		}
	}
	return mt;
}

int* matrixInit(int dimension){
	int *matrix = (int*)malloc(dimension*dimension*sizeof(int));
	for(int i=0;i<dimension*dimension;i++)
		matrix[i] = 0;
	return matrix;
}

int* sequentialMM(int *matrix1, int *matrix2, int dimension){
	int* m = (int*)malloc(dimension*dimension*sizeof(int));
	for(int i=0; i<dimension;i++){
		for(int j=0; j<dimension;j++){
			for(int k=0;k<dimension;k++)
				m[i*dimension+j] += matrix1[i*dimension+k]*matrix2[j*dimension+k];
		}
	}
	return m;
}

int compare(int* matrix1, int* matrix2, int dimension){
	for(int i=0; i<dimension*dimension; i++){
		if(matrix1[i]-matrix2[i] == 0)
			continue;
		else
			return 0;
	}
	return 1;
}


int main(){
//	int matrix_size;
	//printf("Please input the square matrix size!\n");
	//scanf(&matrix_size);
	//printf("matrix size is %d\n", matrix_size);


	struct timespec start, end;

	int *m1 = matrixgenerate(MATRIX_SIZE);
	int *m2 = matrixgenerate(MATRIX_SIZE);
	int *transposed_m2 = matrixTranspose(m2, MATRIX_SIZE);
	int *openmp_matrix = matrixInit(MATRIX_SIZE);

	/* printf("***** Matrix Display for m1 *****\n"); */
	/* matrixdisplay(m1,MATRIX_SIZE); */

	/* printf("***** Matrix Display for m2 *****\n"); */
	/* matrixdisplay(m2,MATRIX_SIZE); */

	/* printf("***** Matrix Display for transposed m2 *****\n"); */
	/* matrixdisplay(transposed_m2, MATRIX_SIZE); */

	/* printf("***** Matrix Display for openmp matrix *****\n"); */
	/* matrixdisplay(openmp_matrix,MATRIX_SIZE); */


	clock_gettime(CLOCK_MONOTONIC_RAW, &start);
	int* sequential_matrix = sequentialMM(m1, transposed_m2, MATRIX_SIZE);
	clock_gettime(CLOCK_MONOTONIC_RAW, &end);
	uint64_t diff = 1000000000L * (end.tv_sec - start.tv_sec) + end.tv_nsec - start.tv_nsec;
	printf("Elapsed process sequential time = %llu nanoseconds\n", (long long unsigned int)diff);


	clock_gettime(CLOCK_MONOTONIC_RAW, &start);
	//omp_set_num_threads(4);
	#pragma omp parallel for
	for(int i=0; i<MATRIX_SIZE;i++){
		for(int j=0; j<MATRIX_SIZE;j++){
			for(int k=0;k<MATRIX_SIZE;k++)
				openmp_matrix[i*MATRIX_SIZE+j] += m1[i*MATRIX_SIZE+k]*transposed_m2[j*MATRIX_SIZE+k];
		}
	}
	clock_gettime(CLOCK_MONOTONIC_RAW, &end);
	diff = 1000000000L * (end.tv_sec - start.tv_sec) + end.tv_nsec - start.tv_nsec;
	printf("Elapsed process sequential time = %llu nanoseconds\n", (long long unsigned int)diff);


	/* printf("***** Matrix Display for sequential matrixm *****\n"); */
	/* matrixdisplay(sequential_matrix, MATRIX_SIZE); */

	/* printf("***** Matrix Display for openmp matrixm *****\n"); */
	/* matrixdisplay(openmp_matrix, MATRIX_SIZE); */

	if(compare(sequential_matrix, openmp_matrix,MATRIX_SIZE))
		printf("Successful!\n");
	else
		printf("Failed!\n");

	return 0;
}

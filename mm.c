#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>

#define MAX_VAL 1000
//#define MATRIX_SIZE 10

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

// transpose a matrix
int* matrixTranspose(int *matrix, int dimension){
	int *mt = (int*)malloc(dimension*dimension*sizeof(int));
	for(int i=0;i<dimension;i++){
		for(int j=0;j<dimension;j++){
			mt[i*dimension+j] = matrix[j*dimension+i];
		}
	}
	return mt;
}

// init all elements of the matrix as 0
int* matrixInit(int dimension){
	int *matrix = (int*)malloc(dimension*dimension*sizeof(int));
	for(int i=0;i<dimension*dimension;i++)
		matrix[i] = 0;
	return matrix;
}

// matrix multiplication with sequential process
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

// compare to make sure openmp is correct
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
	printf("***** Matrix Multiplication with Openmp \n"
		   "***** First input the size of matrix \n"
		   "***** Then, input the number of threads you want to create\n"
		   "***** NOTE: proper matrix size and thread count make Openmp ourperform sequential processing!\n");

	int matrix_size, thread_cnt;
	printf("Please input the square matrix size!\n");
	scanf("%d", &matrix_size);
	printf("Please input the number of threads!\n");
	scanf("%d", &thread_cnt);

	struct timespec start, end;

	int *m1 = matrixgenerate(matrix_size);
	int *m2 = matrixgenerate(matrix_size);
	int *transposed_m2 = matrixTranspose(m2, matrix_size);
	int *openmp_matrix = matrixInit(matrix_size);

	// this code is used to test the correctness of the code
	/* printf("***** Matrix Display for m1 *****\n"); */
	/* matrixdisplay(m1,matrix_size); */
	/* printf("***** Matrix Display for m2 *****\n"); */
	/* matrixdisplay(m2,matrix_size); */
	/* printf("***** Matrix Display for transposed m2 *****\n"); */
	/* matrixdisplay(transposed_m2, matrix_size); */
	/* printf("***** Matrix Display for openmp matrix *****\n"); */
	/* matrixdisplay(openmp_matrix,matrix_size); */

	clock_gettime(CLOCK_MONOTONIC_RAW, &start);
	int* sequential_matrix = sequentialMM(m1, transposed_m2, matrix_size);
	clock_gettime(CLOCK_MONOTONIC_RAW, &end);
	uint64_t diff = 1000000000L * (end.tv_sec - start.tv_sec) + end.tv_nsec - start.tv_nsec;
	printf("Elapsed process sequential time = %llu nanoseconds\n", (long long unsigned int)diff);


	clock_gettime(CLOCK_MONOTONIC_RAW, &start);
	omp_set_num_threads(thread_cnt);
	#pragma omp parallel for
	for(int i=0; i<matrix_size;i++){
		for(int j=0; j<matrix_size;j++){
			for(int k=0;k<matrix_size;k++)
				openmp_matrix[i*matrix_size+j] += m1[i*matrix_size+k]*transposed_m2[j*matrix_size+k];
		}
	}
	clock_gettime(CLOCK_MONOTONIC_RAW, &end);
	diff = 1000000000L * (end.tv_sec - start.tv_sec) + end.tv_nsec - start.tv_nsec;
	printf("Elapsed process sequential time = %llu nanoseconds\n", (long long unsigned int)diff);

	// this code is used to test the correctness of the code
	/* printf("***** Matrix Display for sequential matrixm *****\n"); */
	/* matrixdisplay(sequential_matrix, matrix_size); */
	/* printf("***** Matrix Display for openmp matrixm *****\n"); */
	/* matrixdisplay(openmp_matrix, matrix_size); */

	if(compare(sequential_matrix, openmp_matrix,matrix_size))
		printf("Successful!\n");
	else
		printf("Failed!\n");

	return 0;
}

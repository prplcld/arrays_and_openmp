#include "pch.h"
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <ctime>
#include <stdlib.h>
#include <time.h>

#define N 1000

int A[N][N];
int B[N][N];
int C[N][N];

int main()
{
	int i, j, k;
	double elapsed, elapsed1;
	omp_set_num_threads(omp_get_num_procs());
	for (i = 0; i < N; i++) {
		for (j = 0; j < N; j++)
		{
			A[i][j] = rand() % 10 + 1;
			B[i][j] = rand() % 10 + 1;
		}
	}
	clock_t begin1 = clock();
	for (i = 0; i < N; ++i) {
		for (j = 0; j < N; ++j) {
			for (k = 0; k < N; ++k) {
				C[i][j] += A[i][k] * B[k][j];
			}
		}
	}
	clock_t end1 = clock();
	elapsed1 = double(end1 - begin1) / 1000;
	printf("elapsed time = %f seconds without threads\n ", elapsed1);

	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			C[i][j] = 0;
		}
	}

	clock_t begin = clock();
	#pragma omp parallel for private(i,j,k) shared(A,B,C)
	for (i = 0; i < N; ++i) {
		for (j = 0; j < N; ++j) {
			for (k = 0; k < N; ++k) {
				C[i][j] += A[i][k] * B[k][j];
			}
		}
	}

	clock_t end = clock();
	elapsed = double(end - begin) / 1000;
	printf("elapsed time = %f seconds with threads\n ", elapsed);
	printf("%f", elapsed1 / elapsed);
}
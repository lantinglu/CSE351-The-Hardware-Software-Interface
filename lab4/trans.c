/*
 * CSE 351 Lab 4 (Caches and Cache-Friendly Code)
 * Part 2 - Optimizing Matrix Transpose
 *
 * Name(s): Lanting Lu 
 * NetID(s): llu3
 *
 * Each transpose function must have a prototype of the form:
 * void trans(int M, int N, int A[M][N], int B[N][M]);
 * and compute B = A^T.
 *
 * A transpose function is evaluated by counting the number of misses
 * on a 1 KiB direct mapped cache with a block size of 32 bytes.
 */

#include <stdio.h>
#include "support/cachelab.h"

int is_transpose(int M, int N, int A[M][N], int B[N][M]);


/*
 * transpose_submit - This is the transpose function that you will be graded
 *     on. Do not change the description string "Transpose submission", as the
 *     driver searches for that string to identify the transpose function to be
 *     graded.
 */
char transpose_submit_desc[] = "Transpose submission";
void transpose_submit(int M, int N, int A[M][N], int B[N][M]) {
    int blockSize;
    if (M == 32){
        int temp;
        int index;
        blockSize = 8;
        for (int i = 0; i < M; i += blockSize) {
            for (int j = 0; j < M; j += blockSize) {
                for (int k = i; k < i + blockSize; k++) {
                    for (int l = j; l < j + blockSize; l++) {
                        if (k != l) {
                            B[l][k] = A[k][l];
                        } else {
                            temp = A[k][l];
                            index = k;
                        }
                    }
                    if (i == j) {
                        B[index][index] = temp;
                    }
                }
            }
        }
    }
    if (M == 64){
        blockSize = 4;
        for(int i = 0; i < M; i += blockSize){
            for(int j = 0; j < M; j += blockSize){
                for(int k = i; k < i + blockSize; k++){
                    int temp1 = A[k][j];
                    int temp2 = A[k][j+1];
                    int temp3 = A[k][j+2];
                    int temp4 = A[k][j+3];
                    B[j][k] = temp1;
                    B[j+1][k] = temp2;
                    B[j+2][k] = temp3;
                    B[j+3][k] = temp4;
                }
            }
        }
    }
}


// You can define additional transpose functions below. We've defined a simple
// one below to help you get started.

/*
 * trans - A simple baseline transpose function, not optimized for the cache.
 */
char trans_desc[] = "Simple row-wise scan transpose";
void trans(int M, int N, int A[M][N], int B[N][M]) {
    int i, j, tmp;

    for (i = 0; i < M; i++) {
        for (j = 0; j < N; j++) {
            tmp = A[i][j];
            B[j][i] = tmp;
        }
    }

}


/*
 * registerFunctions - This function registers your transpose
 *     functions with the driver.  At runtime, the driver will
 *     evaluate each of the registered functions and summarize their
 *     performance. This is a handy way to experiment with different
 *     transpose strategies.
 */
void registerFunctions() {
    /* Register your solution function */
    registerTransFunction(transpose_submit, transpose_submit_desc);

    /* Register any additional transpose functions */
    registerTransFunction(trans, trans_desc);

}


/*
 * is_transpose - This helper function checks if B is the transpose of
 *     A. You can check the correctness of your transpose by calling
 *     it before returning from the transpose function.
 */
int is_transpose(int M, int N, int A[M][N], int B[N][M]) {
    int i, j;

    for (i = 0; i < M; i++) {
        for (j = 0; j < N; ++j) {
            if (A[i][j] != B[j][i]) {
                return 0;
            }
        }
    }
    return 1;
}

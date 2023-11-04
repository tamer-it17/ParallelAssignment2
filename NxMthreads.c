#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define ROWS 750
#define COLS 600
#define INNER_DIM 650
#define NUM_THREADS 1

struct ThreadArgs {
    int start_row;
    int end_row;
    int (*matA)[COLS];
    int (*matB)[INNER_DIM];
    int (*matC)[INNER_DIM];
};

void* multiply_matrices(void* arg) {
    struct ThreadArgs* args = (struct ThreadArgs*) arg;
    for(int i = args->start_row; i < args->end_row; i++) {
        for(int j = 0; j < INNER_DIM; j++) {
            int sum = 0;
            for(int k = 0; k < COLS; k++) {
                sum += args->matA[i][k] * args->matB[k][j];
            }
            args->matC[i][j] = sum;
        }
    }
    pthread_exit(NULL);
}

int main() {
    int (*matA)[COLS] = malloc(sizeof(int[ROWS][COLS]));
    int (*matB)[INNER_DIM] = malloc(sizeof(int[COLS][INNER_DIM]));
    int (*matC)[INNER_DIM] = malloc(sizeof(int[ROWS][INNER_DIM]));
    srand(time(NULL));
    for(int i = 0; i < ROWS; i++) {
        for(int j = 0; j < COLS; j++) {
            matA[i][j] = rand() % 10;
        }
    }
    for(int i = 0; i < COLS; i++) {
        for(int j = 0; j < INNER_DIM; j++) {
            matB[i][j] = rand() % 10;
        }
    }

    clock_t start_time = clock();
    pthread_t threads[NUM_THREADS];
    struct ThreadArgs args[NUM_THREADS];
    for(int i = 0; i < NUM_THREADS; i++) {
        args[i].start_row = i * ROWS / NUM_THREADS;
        args[i].end_row = (i + 1) * ROWS / NUM_THREADS;
        args[i].matA = matA;
        args[i].matB = matB;
        args[i].matC = matC;
        pthread_create(&threads[i], NULL, multiply_matrices, &args[i]);
    }
    for(int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }
    clock_t end_time = clock();

    printf("Matrix multiplication done successfully.\n");
    printf("Execution time over %d threads: %f seconds.\n",NUM_THREADS, (double)(end_time - start_time) / CLOCKS_PER_SEC);

    return 0;
}
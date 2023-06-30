#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <mpi.h>

#define NOT_READ -1
#define MIN_ARRAY_SIZE 1
#define MAX_ARRAY_SIZE 64

void print_usage_message() {
    printf("Uso: mpirun -np <num_procs> ./broadcast --array_size <array_size> [--custom]\n");
}

void parse_arguments(int argc, char *argv[], int *array_size_ptr, bool *is_custom_bcast_ptr) {
    int ret;

    if (argc != 3 && argc != 4) {
        print_usage_message();
        exit(EXIT_FAILURE);
    }

    *array_size_ptr = NOT_READ;
    *is_custom_bcast_ptr = false;

    for (int i = 0; i < argc; i++) {
        if (strcmp(argv[i], "--array_size") == 0) {
            ret = sscanf(argv[i+1], "%d", array_size_ptr);
            if (ret == 0 || ret == EOF) {
                fprintf(stderr, "Error reading array_size\n");
                exit(EXIT_FAILURE);
            }
        }

        if (strcmp(argv[i], "--custom") == 0) {
            *is_custom_bcast_ptr = true;
        }
    }

    if (*array_size_ptr == NOT_READ) {
        print_usage_message();
        exit(EXIT_FAILURE);
    }

    if (*array_size_ptr < MIN_ARRAY_SIZE || *array_size_ptr > MAX_ARRAY_SIZE) {
        printf("Invalid array_size %d (min=%d, max=%d)\n",
            *array_size_ptr, MIN_ARRAY_SIZE, MAX_ARRAY_SIZE);
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char *argv[]) {
    int rank, num_procs;

    // Inicializacao do ambiente de execucao MPI
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int array_size; 
    bool is_custom_bcast;
    parse_arguments(argc, argv, &array_size, &is_custom_bcast);

    printf("%d out of %d array_size=%d custom=%d argc=%d\n", 
        rank, num_procs, array_size, is_custom_bcast, argc);

	MPI_Finalize();
    return 0;
}

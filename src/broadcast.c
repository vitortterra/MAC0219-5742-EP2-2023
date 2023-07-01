#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sys/time.h>
#include <mpi.h>

#define NOT_READ -1
#define MIN_ARRAY_SIZE 1
#define MAX_ARRAY_SIZE 64

// Procedimento auxiliar para inicializar o gerador de 
// numeros pseudo-aleatorios (PRNG).
// Utiliza-se o Unix time em microssegundos (us) como seed.
void initialize_prng() {
    struct timeval tv;
    long total_us;

    gettimeofday(&tv, NULL);
    total_us = tv.tv_sec * 1000000 + tv.tv_usec;

    srand(total_us);
}

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

int custom_bcast(void *buffer, int count, MPI_Datatype datatype, int root, MPI_Comm comm ) {
    // SEU CODIGO DO EP2 AQUI

    return MPI_SUCCESS;
}

int main(int argc, char *argv[]) {
    int rank;

    // Inicializacao do ambiente de execucao MPI
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int array_size; 
    bool is_custom_bcast;
    parse_arguments(argc, argv, &array_size, &is_custom_bcast);

    int *buf = (int *) malloc(array_size * sizeof(int));
    if (rank == 0) {
        initialize_prng();
        for (int i = 0; i < array_size; i++)
            buf[i] = rand();
    }

    if (is_custom_bcast)
        custom_bcast(buf, array_size, MPI_INT, 0, MPI_COMM_WORLD);
    else
        MPI_Bcast(buf, array_size, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0)
        printf("Data sent from root, rank=%d\n", rank);
    else
        printf("Data received at rank=%d\n", rank);

    for (int i = 0; i < array_size; i++) 
        printf("%d ", buf[i]);
    printf("\n\n");

	MPI_Finalize();
    return 0;
}

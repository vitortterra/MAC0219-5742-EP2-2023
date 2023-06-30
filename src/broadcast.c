#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <mpi.h>

int main(int argc, char *argv[]) {
    int rank, num_procs;

    // Inicializacao do ambiente de execucao MPI
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    printf("%d out of %d\n", rank, num_procs);

	MPI_Finalize();
    return 0;
}

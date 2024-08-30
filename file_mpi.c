#include <stdio.h>
#include <mpi.h>
#include <unistd.h>

typedef struct {
    int x;
    double y;
} Data;

int main(int argc, char *argv[]) {
    int rank, size;
    char hostname[256];
    MPI_Init(&argc, &argv);
    MPI_Comm comm = MPI_COMM_WORLD;
    MPI_Comm_rank(comm, &rank);
    MPI_Comm_size(comm, &size);

    // Obtém o nome do host
    gethostname(hostname, sizeof(hostname));

    Data data;
    data.x = 1;
    data.y = 2.0;

    if (rank == 0) {
        // Master process
        for (int i = 1; i < size; i++) {
            MPI_Send(&data, sizeof(Data), MPI_BYTE, i, i, comm);
        }
        printf("Process %d sent data: {%d, %f}\n", rank, data.x, data.y);
    } else {
        // Worker process
        MPI_Recv(&data, sizeof(Data), MPI_BYTE, 0, rank, comm, MPI_STATUS_IGNORE);
        printf("Process %d received data: {%d, %f}\n", rank, data.x, data.y);
    }

    // Imprime o rank e o nome do host
    printf("Process %d of %d is running on host %s\n", rank, size, hostname);

    MPI_Finalize();
    return 0;
}


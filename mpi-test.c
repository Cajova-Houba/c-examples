#include <stdio.h>
#include <mpi.h>

int main(int argc, char** argv){
	int process_Rank, size_Of_Cluster;

	// init MPI env
    MPI_Init(&argc, &argv);

    // cluster size
    MPI_Comm_size(MPI_COMM_WORLD, &size_Of_Cluster);

    // cluster rank
    MPI_Comm_rank(MPI_COMM_WORLD, &process_Rank);

    printf("Hello World from process %d of %d\n", process_Rank, size_Of_Cluster);
    return 0;
}
/***********************************************************************************************
                                    PRINTING PROCESS RANK
************************************************************************************************/

#include<stdio.h>
#include<mpi.h>


int main(int argc, char **argv)
{
    int myRank, size;
    int ierror, i, rank_sum = 0;
    MPI_Request requestHandle;

    // Initializing MPI Process
    MPI_Init(&argc, &argv);

    // Computing size
    ierror=MPI_Comm_size(MPI_COMM_WORLD,&size);
    ierror=MPI_Comm_rank(MPI_COMM_WORLD,&myRank);

    // Saves the reduced sum to zero
    MPI_Ireduce(&myRank, &rank_sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD, &requestHandle);
    MPI_Wait(&requestHandle, MPI_STATUS_IGNORE);

    // Broadcasts data to all from 0 process
    // Here we are not starting from all nodes
    MPI_Bcast(&rank_sum, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // Printing rank sum
    printf ("Process %i:\tSum = %i\n", myRank, rank_sum);

    // Terminal statement
    MPI_Finalize();

    return 0;
}

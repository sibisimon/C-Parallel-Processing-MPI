/***********************************************************************************************
                                        RING COMMUNICATION
************************************************************************************************/

#include<stdio.h>
#include<mpi.h>


int main(int argc, char **argv)
{
    // Initializing variables
    int myRank, size;
    int send, recieve;
    int sendBuffer[2];
    int receivBuffer[2];

    int neighbour_right, neighbour_left;
    int rank_sum, i, ierror;

    MPI_Status recvStatus;
    MPI_Request requestHandle;

    // Initializing MPI Process
    MPI_Init(&argc, &argv);

    // Taking size and process rank
    ierror=MPI_Comm_size(MPI_COMM_WORLD,&size);
    ierror=MPI_Comm_rank(MPI_COMM_WORLD,&myRank);

    // Finding neighbour of current rank
    neighbour_right = (myRank+1) % size;
    neighbour_left = (myRank-1+size) % size;

    // Initializing send buffer
    sendBuffer[0] = myRank;
    sendBuffer[1] = myRank;

    while(1)
    {
        // Sending buffer to the neighbour
        MPI_Issend(sendBuffer, 2, MPI_INT, neighbour_right,
        100, MPI_COMM_WORLD, &requestHandle);

        // Receiving buffer
        MPI_Recv(receivBuffer, 2, MPI_INT, neighbour_left,
        100, MPI_COMM_WORLD, &recvStatus);

        // Waiting till the message is received
        MPI_Wait(&requestHandle, &recvStatus);

        // If the message reaches originator terminating the loop
        if (receivBuffer[0] == myRank)
            break;

        // Updating the sum
        sendBuffer[1] = receivBuffer[1] + myRank;
        sendBuffer[0] = receivBuffer[0];

        rank_sum = sendBuffer[1];

    }

    // Printing the process sum
    printf ("Process %i:\tSum = %i\n", myRank, rank_sum);

    // Terminal statement
    MPI_Finalize();

    return 0;
}

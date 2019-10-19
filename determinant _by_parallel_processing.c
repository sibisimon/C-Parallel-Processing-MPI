/***********************************************************************************************
                                 DETERMINANT PARALLEL PROCESSING
************************************************************************************************/

// Including header files required for the program
#include <stdio.h>
#include<mpi.h>
#include<math.h>


// Declaring prototypes of functions used
double FindDeterminantOf3By3(double [3][3]);
double FindDeterminantOf4By4(double [4][4]);
void GenerateMatrix(double [5][5]);
void Get4by4SubMatrix(double [5][5], double [4][4], int);
void Get3by3SubMatrix(double [4][4], double [3][3], int);
void DisplayMatrix(double [5][5]);

// Main function
int main(int argc, char **argv)
{
    double mat[5][5];
    double determinant;
    int myRank, size;
    int ierror;
    double subMat[4][4];
    double det5by5;
    double det;

    MPI_Init(&argc, &argv);

    ierror=MPI_Comm_size(MPI_COMM_WORLD,&size);
    ierror=MPI_Comm_rank(MPI_COMM_WORLD,&myRank);

    // Checking for process 5
    if(size != 5){
         MPI_Finalize();
         printf("Enter process number as 5 --------> ");
         printf ("Process %i: exited\n", myRank);
         exit(0);
    }

    // Generating Given matrix
    GenerateMatrix(mat);


    // Getting sub matrices from 5 x 5 matrix
    // Parameters are 1. matrix 2. Matrix to store 3. Column to avoid
    Get4by4SubMatrix(mat, subMat, myRank);

    // Finding determinant of 4 by 4 matrix
    det =  pow(-1, myRank) * mat[0][myRank] * FindDeterminantOf4By4(subMat);

    // MPI Reduce for summing variables individual 4 X 4 determinant will be added to root 0 and will be displayed
    MPI_Reduce(&det, &det5by5, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    printf ("\n\nProcess %i:\t Determinant = %0.16lf\n", myRank, det);

    if (myRank == 0){

        // Displaying determinant of matrix
        printf("\n\n\n-----------------------------------------------------------------------------------");
        printf("\nDETERMINANT OF MATRIX: %0.16lf", det5by5);
        printf("\n-----------------------------------------------------------------------------------\n\n\n");
    }

    MPI_Finalize();

}

// (a)
// Function for finding determinant of a 3 by 3 function
double FindDeterminantOf3By3(double mat[3][3])
{
    double detFinal, detA, detB, detC;
    detA = mat[0][0] * ((mat[1][1] * mat[2][2]) - (mat[2][1] * mat[1][2]));
    detB = mat[0][1] * ((mat[1][0] * mat[2][2]) - (mat[2][0] * mat[1][2]));
    detC = mat[0][2] * ((mat[1][0] * mat[2][1]) - (mat[2][0] * mat[1][1]));

    // Calculating determinant
    detFinal = detA - detB + detC;

    return detFinal;
}


double FindDeterminantOf4By4(double mat[4][4])
{
    double subMatA[3][3], subMatB[3][3], subMatC[3][3], subMatD[3][3];
    double detA, detB, detC, detD, finalDet;

    // Getting four sub matrices from Hilbert matrix
    // Parameters are 1. matrix 2. Matrix to store 3. Column to avoid
    Get3by3SubMatrix(mat, subMatA, 0);
    Get3by3SubMatrix(mat, subMatB, 1);
    Get3by3SubMatrix(mat, subMatC, 2);
    Get3by3SubMatrix(mat, subMatD, 3);

    // Finding determinant of Hilbert matrix
    detA = mat[0][0] * FindDeterminantOf3By3(subMatA);
    detB = mat[0][1] * FindDeterminantOf3By3(subMatB);
    detC = mat[0][2] * FindDeterminantOf3By3(subMatC);
    detD = mat[0][3] * FindDeterminantOf3By3(subMatD);
    finalDet = detA - detB + detC - detD;

    return finalDet;
}


// (b)
// Displaying Matrix with the help of nested loops
void GenerateMatrix(double mat[5][5])
{
    int row, col;

    for(row=0;row<5;row++)
    {
		for(col=0;col<5;col++)
		{
		    if (col == row){
                mat[row][col]=(double)1.0/((row+1)+(col+1)-1.0);
		    }
			else{
                mat[row][col]= (-1) * (double)1.0/((row+1)+(col+1)-1.0);
			}
		}
	}
}

// Function for generating sub matrix of Hilbert matrix
void Get3by3SubMatrix(double mat[4][4], double subMat[3][3], int avoidCol)
{
    int sRow=0, sCol=0, row, col;

    for (row = 1; row <= 3; row++)
    {
        for (col = 0; col <= 3; col++)
        {   // Avoiding column
            if (col != avoidCol)
            {
                // Writing sub matrix to passed matrix
                subMat[sRow][sCol++] = mat[row][col];

                // Incrementing row of sub matrix if max size of col is reached and making col = 0
                if (sCol == 3)
                {
                    sCol = 0;
                    sRow++;
                }
            }
        }
    }
}

void Get4by4SubMatrix(double mat[5][5], double subMat[4][4], int avoidCol)
{
    int sRow=0, sCol=0, row, col;
    for (row = 1; row <= 4; row++)
    {
        for (col = 0; col <= 4; col++)
        {   // Avoiding column
            if (col != avoidCol)
            {
                // Writing sub matrix to passed matrix
                subMat[sRow][sCol++] = mat[row][col];

                // Incrementing row of sub matrix if max size of col is reached and making col = 0
                if (sCol == 4)
                {
                    sCol = 0;
                    sRow++;
                }
            }
        }
    }
}

// Display method for displaying a 4 * 4 matrix
void DisplayMatrix(double mat[5][5])
{
    int rowCount, columnCount  = 0;
    printf("\n\n--------------------------------------- MATRIX ------------------------------------\n\n");
    for(rowCount = 0;rowCount <= 4;rowCount++ )
    {
        for(columnCount = 0;columnCount <=4;columnCount++)
        {
            printf(" %-10f ",mat[rowCount][columnCount]);
        }
        printf("\n");
    }
    printf("\n-----------------------------------------------------------------------------------");
}

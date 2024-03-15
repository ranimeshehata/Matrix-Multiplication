#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <errno.h>
#include <pthread.h>

#define SIZE 20

#define BRED "\e[1;31m"
#define BGRN "\e[1;32m"
#define BYEL "\e[1;33m"
#define reset "\e[0m"

FILE *fileA;
FILE *fileB;
FILE *fileCMatrix;
FILE *fileCRow;
FILE *fileCElement;

int rowA, colA, rowB, colB, rowC, colC;
int matrixA[SIZE][SIZE], matrixB[SIZE][SIZE], matrixCMat[SIZE][SIZE], matrixCRow[SIZE][SIZE], matrixCElement[SIZE][SIZE];

char dataA[SIZE], dataB[SIZE];

struct Data
{
    int row;
    int col;
};

char *input1;
char *input2;
char *output_matrix;
char *output_row;
char *output_element;

void readInput()
{
    // Read file 1
    fileA = fopen(input1, "r");
    // Check if file is empty
    if (fileA == NULL)
    {
        printf(BRED "Error opening file: %s\n" reset, strerror(errno));
        exit(1);
    }
    // Read the first line of the file
    fgets(dataA, SIZE, fileA);
    // dataA has the no of rows and columns of matrix A
    sscanf(dataA, "row=%d col=%d", &rowA, &colA);
    // Read the matrix A
    for (int i = 0; i < rowA; i++)
    {
        for (int j = 0; j < colA; j++)
            fscanf(fileA, "%d", &matrixA[i][j]);
    }
    fclose(fileA);

    // Read file 2
    fileB = fopen(input2, "r");
    // Check if file is empty
    if (fileB == NULL)
    {
        printf(BRED "Error opening file: %s\n" reset, strerror(errno));
        exit(1);
    }
    // Read the first line of the file
    fgets(dataB, SIZE, fileB);
    // dataB has the no of rows and columns of matrix B
    sscanf(dataB, "row=%d col=%d", &rowB, &colB);
    // Read the matrix B
    for (int i = 0; i < rowB; i++)
    {
        for (int j = 0; j < colB; j++)
            fscanf(fileB, "%d", &matrixB[i][j]);
    }
    fclose(fileB);
}

void *multiplyElement(void *arg)
{
    struct Data *data = (struct Data *)arg;
    int row = data->row;
    int col = data->col;
    int sum = 0;
    for (int i = 0; i < colA; i++)
    {
        sum += matrixA[row][i] * matrixB[i][col];
    }
    matrixCElement[row][col] = sum;
    return NULL;
}

void *multiplyRow(void *arg)
{
    int rowNo = *((int *)arg);
    for (int i = 0; i < colB; i++)
    {
        int sum = 0;
        for (int j = 0; j < colA; j++)
        {
            sum += matrixA[rowNo][j] * matrixB[j][i];
        }
        matrixCRow[rowNo][i] = sum;
    }
    return NULL;
}

void *multiplyMatrix(void *arg)
{
    for (int i = 0; i < rowA; i++)
    {
        for (int j = 0; j < colB; j++)
        {
            int sum = 0;
            for (int k = 0; k < colA; k++)
            {
                sum += matrixA[i][k] * matrixB[k][j];
            }
            matrixCMat[i][j] = sum;
        }
    }
    return NULL;
}

// matrix multiplication using pthreads
void method1()
{
    pthread_t t_matrix;
    struct timeval start, stop;
    // start the timer
    gettimeofday(&start, NULL);
    pthread_create(&t_matrix, NULL, &multiplyMatrix, NULL);
    pthread_join(t_matrix, NULL);

    // write the result to the file
    fileCMatrix = fopen(output_matrix, "w");
    fprintf(fileCMatrix, "Method 1: A thread per matrix\n");
    fprintf(fileCMatrix, "row=%d col=%d\n", rowC, colC);
    for (int i = 0; i < rowC; i++)
    {
        for (int j = 0; j < colC; j++)
        {
            fprintf(fileCMatrix, "%d ", matrixCMat[i][j]);
        }
        fprintf(fileCMatrix, "\n");
    }
    fclose(fileCMatrix);
    // stop the timer
    gettimeofday(&stop, NULL);
    printf(BGRN "Number of threads taken for multiplication per matrix: (%d)\n" reset, 1);
    // calculate the time taken
    printf(BGRN "Time taken for multiplication per matrix: %ld microseconds\n" reset, (stop.tv_usec - start.tv_usec));
    printf("---------------------------------------------------------------------------------\n");
}

// row multiplication using pthreads
void method2()
{
    pthread_t t_row[rowA];
    struct timeval start, stop;
    // start the timer
    gettimeofday(&start, NULL);
    // pass the row number to the thread function by value
    for (int i = 0; i < rowA; i++){
        // allocate memory for each row number
        int *rowNum = malloc(sizeof(*rowNum));
        // assign the current row number to the allocated memory
        *rowNum = i;
        pthread_create(&t_row[i], NULL, &multiplyRow, (void *)rowNum);
    }
    for (int i = 0; i < rowA; i++)
        pthread_join(t_row[i], NULL);

    // write the result to the file
    fileCRow = fopen(output_row, "w");
    fprintf(fileCRow, "Method 2: A thread per row\n");
    fprintf(fileCRow, "row=%d col=%d\n", rowC, colC);
    for (int i = 0; i < rowC; i++)
    {
        for (int j = 0; j < colC; j++)
        {
            fprintf(fileCRow, "%d ", matrixCRow[i][j]);
        }
        fprintf(fileCRow, "\n");
    }
    fclose(fileCRow);
    // stop the timer
    gettimeofday(&stop, NULL);
    printf(BYEL "Number of threads taken for multiplication per row: (%d)\n" reset, rowA);
    // calculate the time taken
    printf(BYEL "Time taken for multiplication per row: %ld microseconds\n" reset, (stop.tv_usec - start.tv_usec));
    printf("---------------------------------------------------------------------------------\n");
}

void method3()
{
    pthread_t t_element[rowA * colB];
    struct timeval start, stop;
    // start the timer
    gettimeofday(&start, NULL);
    int count = 0;
    for (int i = 0; i < rowA; i++)
    {
        for (int j = 0; j < colB; j++)
        {
            // pass the row and column number to the thread function by value
            struct Data *data = (struct Data *)malloc(sizeof(struct Data));
            data->row = i;
            data->col = j;
            pthread_create(&t_element[count], NULL, &multiplyElement, (void *)data);
            count++;
        }
    }
    for (int i = 0; i < rowA * colB; i++)
    {
        pthread_join(t_element[i], NULL);
    }

    // write the result to the file
    fileCElement = fopen(output_element, "w");
    fprintf(fileCElement, "Method 3: A thread per element\n");
    fprintf(fileCElement, "row=%d col=%d\n", rowC, colC);
    for (int i = 0; i < rowC; i++)
    {
        for (int j = 0; j < colC; j++)
        {
            fprintf(fileCElement, "%d ", matrixCElement[i][j]);
        }
        fprintf(fileCElement, "\n");
    }
    fclose(fileCElement);
    // stop the timer
    gettimeofday(&stop, NULL);
    printf(BRED "Number of threads taken for multiplication per element: (%d)\n" reset, rowA * colB);
    // calculate the time taken
    printf(BRED "Time taken for multiplication per element: %ld microseconds\n" reset, (stop.tv_usec - start.tv_usec));
    printf("---------------------------------------------------------------------------------\n");
}

void files(char *argv[])
{
    input1 = malloc(strlen(argv[1]) + 5);
    // Copy the original string to the new string
    strcpy(input1, argv[1]);
    // Concatenate the extension to the new string
    strcat(input1, ".txt");

    input2 = malloc(strlen(argv[2]) + 5);
    strcpy(input2, argv[2]);
    strcat(input2, ".txt");

    output_matrix = malloc(strlen(argv[3]) + 16);
    strcpy(output_matrix, argv[3]);
    strcat(output_matrix, "_per_matrix.txt");

    output_row = malloc(strlen(argv[3]) + 13);
    strcpy(output_row, argv[3]);
    strcat(output_row, "_per_row.txt");

    output_element = malloc(strlen(argv[3]) + 17);
    strcpy(output_element, argv[3]);
    strcat(output_element, "_per_element.txt");
}

int main(int argc, char *argv[])
{
    // Check if the number of arguments is correct
    if (argc != 4 && argc != 1)
    {
        errno = ENOENT;
        perror(BRED "Error: input invalid! " reset);
        exit(1);
    }
    // default input arguments
    if (argc == 1)
    {
        input1 = "a.txt";
        input2 = "b.txt";
        output_matrix = "c_per_matrix.txt";
        output_row = "c_per_row.txt";
        output_element = "c_per_element.txt";
    }
    // custom input arguments
    if (argc == 4)
        files(argv);

    // Read the input files
    readInput();

    // check if the matrices can be multiplied
    if (colA != rowB)
    {
        printf(BRED "Error: The matrices cannot be multiplied\n" reset);
        exit(1);
    }
    else
    {
        rowC = rowA;
        colC = colB;
        method1();
        method2();
        method3();
    }
    return 0;
}

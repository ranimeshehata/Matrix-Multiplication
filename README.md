# Matrix Multiplication (Multi-Threading)

![MatMul](https://github.com/ranimeshehata/Matrix-Multiplication/assets/121239735/5f105905-a1ba-480e-a1a8-f901938012dc)


## 1. Objectives

* Gettin familiar with thread programming using the [Pthread library](https://hpc-tutorials.llnl.gov/posix/).
* To better understand processes and threads.
## 2. Overview

A multi-threaded [matrix multiplication](https://www.mathsisfun.com/algebra/matrix-multiplying.html) implementation program.

The input to the program is two matrixes A(x*y) and B(y*z) that are read from corresponding text files. The output is a matrix C(x*z) that is written to an output text file.

A parallelized version of matrix multiplication can be done using one of these three methods:

1. A thread computes the output C matrix i.e. without multi-threading. (A thread per matrix).
![per_matrix](https://github.com/ranimeshehata/Matrix-Multiplication/assets/121239735/c31f353f-b5a2-4fe3-b71d-76700c424b57)
2. A thread computes each row in the output C matrix. (A thread per row).
![per_row](https://github.com/ranimeshehata/Matrix-Multiplication/assets/121239735/fe8f5bed-3fb7-4df1-90af-24a061fcb827)
3. A thread computes each element in the output C matrix. (A thread per element).
![per_element](https://github.com/ranimeshehata/Matrix-Multiplication/assets/121239735/ec61f5a7-d692-435e-9004-2a4dc5ecfaeb)

## 3. Comparing the three implementations according to the following:
    Number of threads created and Execution time taken:
    Creating and handling threads requires extra overhead and lots of computation, so, it's not always the ideal solution to go for multi-threading, and there's always a tradeoff.
![WhatsApp Image 2024-03-15 at 5 50 42 AM](https://github.com/ranimeshehata/Matrix-Multiplication/assets/121239735/c53101a5-88ad-4ee1-8d56-4dd52e3ec49d)

The program does the following:

* If the user does not enter the file name, the default is a.txt and b.txt, for input matrixes A and B, and c for output matrices (of all three methods) C. The following example should clarify inputs/outputs files.

    Arguments

        Example: 
            ./exe a b c
        Input files: 
            a.txt 
            b.txt
        Output files: 
            c_per_matrix.txt
            c_per_row.txt
            c_per_element.txt

    No Arguments:

        Example: 
            ./exe
        Input files: 
            a.txt 
            b.txt
        Output files: 
            c_per_matrix.txt
            c_per_row.txt
            c_per_element.txt

    Custom Arguments:

        Example: 
            ./exe x y z
        Input files: 
            x.txt 
            y.txt
        Output files: 
            z_per_matrix.txt
            z_per_row.txt
            z_per_element.txt

* Read the number of rows and columns of the input matrices. They are written in the first line of the file as ”row=x col=y”. The following is an example of the format on an input file.

        row=3 col=5
        1 2 3 4 5
        6 7 8 9 10
        11 12 13 14 15

* Read the input matrices from their corresponding files. Each row is on a separate line, columns are separated by spaces.
* Use threads to calculate the matrix that results from multiplying the input two matrixes.
* Output the resulting matrices in three files (each file represents one method). The following is an example of the format of the three output files.

    c_per_matrix.txt

        Method: A thread per matrix
        row=2 col=2
        1 2
        3 4

    c_per_row.txt

        Method: A thread per row
        row=2 col=2
        1 2
        3 4

    c_per_element.txt

        Method: A thread per element
        row=2 col=2
        1 2
        3 4

    The output matrices are all be the same for all three methods.

* **The number of threads created and the time taken for all the three methods (three different outputs) is printed on the console.**
* Assuming matrix size is no larger than 20 x 20.  

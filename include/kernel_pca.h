// C/C++ example for the CUBLAS (NVIDIA)
// implementation of PCA-GS algorithm
//
// M. Andrecut (c) 2008
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
// includes, cuda
#include "cublas_v2.h"


// This indexing macro is not used internally, but is useful for users contructing data arrays in c or c++ 

// matrix indexing convention for fortran-contiguous arrays
#define ind_f(m, n, num_rows) (((n) * (num_rows)) + (m))


// indexing for c contiguous arrays. This is only used if  a numpy array is c contiguous, then it needs to be converted to fortran contiguous for KernelPCA. 
#define ind_c(m, n, num_cols) (((m) * (num_cols)) + (n))


// useful macro
#define __min__(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a < _b ? _a : _b; })



/*
 Fit a PCA model to the data matrix X, and return the principal components T. The memory for X is not freed in the function, so the user must do that after the call if X is no longer needed. 
 


 input
 h: cublas handle
 K: int - number of principal components to compute
 X: double* - device pointer to data array. The array represents an MxN matrix, where each M elements of X is the ith column of the matrix.
 M: int - number of rows (samples) in X
 N: int - number of columns (features) in X
 verbose: bool - whether or not to display a progress bar in the terminal. This is very useful for large Xs

 return
 T: double* - device pointer to transformed matrix, with the same indexing as X
*/


extern "C" double* dev_fit_transform_d(cublasHandle_t h, int M, int N, double *dX, int K, bool is_c_contiguous);


/*
 Overload of double-precision version.

 Fit a PCA model to the data matrix X, and return the principal components T. The memory for X is not freed in the function, so the user must do that after the call if X is no longer needed. 
 


 input
 h: cublas handle
 K: int - number of principal components to compute
 X: float* - device pointer to data array. The array represents an MxN matrix, where each M elements of X is the ith column of the matrix.
 M: int - number of rows (samples) in X
 N: int - number of columns (features) in X
 verbose: bool - whether or not to display a progress bar in the terminal. This is very useful for large Xs

 return
 T: float* - device pointer to transformed matrix, with the same indexing as X
*/

extern "C" float* dev_fit_transform_f(cublasHandle_t h, int M, int N, float *dX, int K, bool is_c_contiguous);

// functions to switch contiguity of gpu arrays. They alter the original array

// This function is explicitly for converting general pycuda arrays to fortran contiguous with stride [1,1]
extern "C" void c_strided_to_f_contiguous_f(int M, int N, int* strides, float* arr);

// for converting back to c contiguous if the original array was c contiguous
extern "C" void f_to_c_contiguous_f(int M, int N, int* strides ,float* arr);

// same as above, but for double precision
extern "C" void c_strided_to_f_contiguous_d(int M, int N, int* strides, double* arr);
extern "C" void f_to_c_contiguous_d(int M, int N, int* strides, double* arr);

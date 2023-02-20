/* Operating Systems - CS444
 * Spring 2023
 * Iselda Aiello
 * Assignment info:
 * Prompts user input for size of two matrices, fill each matrix with random numbers, and verify they can be multiplied.
 * Then, use pthreads to perform dot product calculations in parallel and print the resulting matrix.
 * Program Desc:
 * Dynamically creates two matrix structs, fills them with random numbers, then prints them to the screen.
 * Their multiplication is performed using a dynamically allocated pthread object and performs the multiplication
 * in parallel, and prints the resulting matrix to the screen.
 * Last Modified: 2/19/23
 * Code Review Link https://drive.google.com/file/d/1-mpSiSofIkdAr0DNv-CtzY_XZMK3De4j/view?usp=sharing
 */

/* includes */
#include<assert.h>
#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>

/* matrix struct */
typedef struct matrix{
  short row, col;
  int **elems;
} matrix;

typedef struct thread_args{
  matrix matrices[2];
  matrix *final_matrix;
  short row, col;
} thread_args;

/* functions */
matrix create_matrix( char );
void print_matrix( matrix, char* );
void *dot_product(void*);

/* main */
int main() {

  // create the matrices
  matrix matrix_one = create_matrix('1');
  matrix matrix_two = create_matrix('2');

  // M1 cols = M2 rows
  assert( matrix_one.col==matrix_two.row );

  // print them to the screen
  print_matrix(matrix_one, "One");
  print_matrix(matrix_two, "Two");

  //initialize final matrix
  matrix matrix_final; matrix_final.row = matrix_one.row; matrix_final.col = matrix_two.col;
  matrix_final.elems = malloc( matrix_final.row * sizeof(int *) );
  assert( matrix_final.elems != NULL );
  for(int i=0; i<matrix_final.row; i++) {
    matrix_final.elems[i] = malloc( matrix_final.col * sizeof(int) );
    assert( matrix_final.elems[i] != NULL );
  }

  // initialize pthreads
  pthread_t **threads = malloc( matrix_final.row * sizeof(pthread_t *) );
  assert( threads != NULL );
  for(int i=0; i<matrix_final.row; i++) {
    threads[i] = malloc( matrix_final.col * sizeof(pthread_t) );
    assert( threads[i] != NULL );
  }

  // create object to be passed through thread function
  thread_args args;
  args.matrices[0]=matrix_one; args.matrices[1]=matrix_two; args.final_matrix = &matrix_final;
  
  // start dotproduct calculations
  for(int i=0; i<matrix_final.row; i++) {
    for(int j=0; j<matrix_final.col; j++) {
      args.row = i; args.col = j; // current row/col for dotproduct
      pthread_create(&threads[i][j], NULL, dot_product, (void *)&args);
      pthread_join(threads[i][j], NULL);
    }
  }

  // join threads
  /*for(int i=0; i<matrix_final.row; i++) {
    for(int j=0; j<matrix_final.col; j++) {
      pthread_join(threads[i][j], NULL);
    }
  }*/

  // print resulting matrix to the screen
  print_matrix(matrix_final, "final");

  // program in C!
  free(matrix_one.elems);
  free(matrix_two.elems);
  free(matrix_final.elems);
  free(threads);

  return 0;
}

/* creates matrices from user input */
matrix create_matrix(char num) {
  matrix matrix;

  // get rows and cols for matrix from stdin
  printf( "Enter the number of rows for matrix %c: ", num);
  scanf( "%d", &matrix.row );
  printf( "Enter the number of columns for matrix %c: ", num);
  scanf( "%d", &matrix.col );

  // initialize matrix row sizes 
  matrix.elems = malloc( matrix.row * sizeof( int* ) );

  // malloc assertion
  assert( matrix.elems != NULL );
  
  // initalize matrix column sizes
  for(int i=0; i<matrix.row; i++) {
    matrix.elems[i] = malloc( matrix.col * sizeof(int) );
    assert( matrix.elems[i] != NULL );
  }
  
  // random initializer
  srand(time(NULL));

  // generate random numbers from 0-99
  for(int i=0; i<matrix.row; i++)
    for(int j=0; j<matrix.col; j++)
      matrix.elems[i][j] = rand() % 100;
  
  return matrix;

}

/* print matrices to terminal */
void print_matrix(matrix matrix, char *name) {
  printf("Matrix %s:\n\t[", name);
  for(int i=0; i<matrix.row; i++) {
    for(int j=0; j<matrix.col; j++) {
      int temp = matrix.elems[i][j];
      if(temp < 10)
        printf("  %d", temp);
      else printf(" %d", temp);
    }
    if(i!=matrix.row-1) printf("\n\t ");
  }
  printf("  ]\n");
}

/* calculate dot product of two vectors */
void *dot_product(void *arguments) {
  thread_args *args = (thread_args *)arguments;
  int temp = 0;

  // multiply M1 row n by M2 col m
  for(int i=0; i<args->matrices[0].col; i++)
    temp += args->matrices[0].elems[args->row][i] * args->matrices[1].elems[i][args->col];

  // assign the temp to the correct element in the final matrix
  args->final_matrix->elems[args->row][args->col] = temp;

  pthread_exit(NULL);
}

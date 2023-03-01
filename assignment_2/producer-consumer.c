/* Operating Systems - CS444
 * Spring 2023
 * Iselda Aiello
 * Assignment info:
 * Program Desc:
 * Last Modified: 2/28/23
 * Code Review Link 
 */

/* includes */
#include<stdio.h>
#include<pthread.h>

/* constants */
#define MAX 500

/* global vars */
pthread_mutex_t mutex;
pthread_cond_t cond_prod, cond_cons;
int buffer = 0;

/* functions */
void *producer(void*);
void *consumer(void*);

/* main */
int main() {

  // create prod/cons threads
  pthread_t prod, cons;

  // intialize mutex to unlocked
  pthread_mutex_init(&mutex, 0);
  // initialize conditions
  pthread_cond_init(&cond_prod, 0);
  pthread_cond_init(&cond_cons, 0);

  // create and start running pthreads
  pthread_create(&prod, NULL, producer, NULL);
  pthread_create(&cons, NULL, consumer, NULL);

  // wait for threads to finish
  pthread_join(prod, 0);
  pthread_join(cons, 0);

  // destroy mutex/cond and end program
  pthread_mutex_destroy(&mutex);
  pthread_cond_destroy(&cond_prod);
  pthread_cond_destroy(&cond_cons);

  return 0;
}

/* producer function */
void *producer(void *args){

  // cycle through MAX times
  for(int i=1; i<=MAX; i++){

    // lock the mutex
    pthread_mutex_lock(&mutex);

    // wait for consumer to finish
    while( buffer != 0 ) pthread_cond_wait(&cond_prod, &mutex);

    // produce some item
    buffer = i;

    // let consumer know producer is done
    pthread_cond_signal(&cond_cons);

    //unlock the mutex
    pthread_mutex_unlock(&mutex);
  }
  // exit pthread
  pthread_exit(NULL);
}

/* consumer function */
void *consumer(void *args){

  // cycle through MAX times
  for(int i=1; i<=MAX; i++){

    // lock the mutex
    pthread_mutex_lock(&mutex);

    // wait for producer to finish
    while( buffer == 0 ) pthread_cond_wait(&cond_cons, &mutex);

    // consume the item
    buffer = 0;

    // let producer know consumer has consumed
    pthread_cond_signal(&cond_prod);

    //unlock the mutex
    pthread_mutex_unlock(&mutex);
  }
  // exit pthread
  pthread_exit(NULL);
}
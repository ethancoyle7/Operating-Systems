////////////////////////////////////////////////////
//                                                //
// Authors    : Ethan Coyle, Jonathan Hogan and   //
//              Dymon Browne                       //
//                                                //
// Teacher    : Dr. Passos                        //
// Class      : Intro to Operating Systems        //
// Assignment : Mini Project 2                    //
//                                                //
// This project includes the use of semaphore and //
// using thread counts  for unix threading        //
////////////////////////////////////////////////////

// include methods
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// number of threads for the program be 5
#define NUM_OF_THREADS 5

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond[5];
int cnt = 0; // count init to 0
// dglobal variables
int TOTAL = 0;
int array[10000];
// void function for semaphore
void *pThreadFunction(void *p) {

  long tid = (long)p;
  pthread_mutex_lock(&mutex);
  // thread waits with semaphore for other to turn up
  if (tid != cnt) {
    pthread_cond_wait(&cond[tid], &mutex);
  }
  printf("TID is : %ld\n", tid);
  // this is one of the two either the one running now or the commented out
  

  for (int i = (tid * 2000); i < ((tid * 2000)+2000); i++) {
    // for (int i = tid * 2000; i > (tid * 2000 - 1); i--)
    // {
    TOTAL = TOTAL + array[i];
  }

  printf("Semaphore is  : %d\n", TOTAL);
  if (cnt < NUM_OF_THREADS - 1) {
    cnt++;
  } else {
    cnt = 0;
  }
  // free up for the next waiting thread
  pthread_cond_signal(&cond[cnt]);
  // the executing thread unlock the mutex
  pthread_mutex_unlock(&mutex);
}
// main driver for program
int main() {
  pthread_t threads[NUM_OF_THREADS];
  // initialize the array of 10000 values
  for (int i = 0; i < 10000; i++) {
    array[i] = i % 257; // initialize i%257 inside of the array of values
  }
  // now we need to create the threads
  for (int i = 0; i < NUM_OF_THREADS; i++) {
    // using pthread create to creat the thread
    pthread_create(&threads[i], NULL, pThreadFunction, (void *)i);
  }
  // joion the threads
  for (int i = 0; i < NUM_OF_THREADS; i++) {
    pthread_join(threads[i], NULL);
  }
  // now we get the TOTAL
  printf("TOTAL is  : %d\n", TOTAL);
  // now we close the threads
  pthread_exit(NULL);
  return 0;
}

// only thing left is make sure values of array are initialized i%257 i think
// that I got that done and then the only other thing is where passos specified
// that the value range cshould be 2000-3999 those are the only two things this
// is lacking and the program will be done

////////////////////////////////////////////////////
//                                                //
// Authors    : Ethan Coyle, Jonathan Hogan and   //
//              Dymon Browne                      //
//                                                //
// Teacher    : Dr. Passos                        //
// Class      : Intro to Operating Systems        //
// Assignment : Mini Project 2                    //
//                                                //
// This project includes the use of semaphore and //
// using thread counts  for unix threading        //
////////////////////////////////////////////////////

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM_OF_THREADS 5
#define SIZE 10000

pthread_mutex_t mx;
pthread_cond_t cond[5];
int myMutex = 1;

int total = 0;
int semaphoreTotal = 0;
int mutexTotal = 0;

int theArray[SIZE];

void wait(S){
    while (S < 1){};
    S--;
}

void post(S){
    S++;
}

void sem_sum(void *thread_id){
    long sum = 0;
    long tid = (long)thread_id;
    long start = ((tid - 1) * 2000);
    long end = start + 2000;

   

    for (int i = start; i < end; i++){
        sum += theArray[i];
    }

    wait(myMutex);
  
    semaphoreTotal += sum;
    printf("Thread %ld: Sum is %ld using a semaphore\n", tid, sum);

    post(myMutex);
}

void mutex_sum(void *thread_id){
    long sum = 0;
    long tid = (long)thread_id;
    long start = ((tid - 1) * 2000);
    long end = start + 2000;

   

    for (int i = start; i < end; i++){
        sum += theArray[i];
    }

    pthread_mutex_lock(&mx);
  
    mutexTotal =mutexTotal+ sum;
    printf("Thread %ld: Sum is %ld using a mutex\n", tid, sum);

    pthread_mutex_unlock(&mx);
}

void no_sem_sum(void *thread_id){
    long sum = 0;
    long tid = (long)thread_id;
    long start = ((tid - 1) * 2000);
    long end = start + 2000;

    for (int i = start; i < end; i++){
        sum += theArray[i];
    }
    total += sum;

    printf("Thread %ld: Sum is %ld without using a semaphore\n", tid, sum);

}

int main()
{
    pthread_t threads[NUM_OF_THREADS];
    int rc;
    long t;
    void *status;
    pthread_attr_t attr;
    printf("\nThe Sums for each are shown below showing w/out\n");
    printf("semaphores, with semaphores and mutex semaphore\n");
    printf("-------------------------------------------------\n\n");
    //pthread_mutex_init(&mx,NULL);
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
    
    for(int i = 1; i < SIZE+1; i++){
        theArray[i-1] = i%257;
    }

    for(int tid = 1; tid <= NUM_OF_THREADS; tid++){
        //Function without a semaphore
        rc = pthread_create(&threads[tid - 1], &attr, no_sem_sum, (void *)tid);
        //Following function uses a semaphore
        rc = pthread_create(&threads[tid - 1], &attr, sem_sum, (void *)tid);
        //Following function uses a mutex
        rc = pthread_create(&threads[tid - 1], &attr, mutex_sum, (void *)tid);
        if (rc){
            printf("ERROR; return code from pthread_create() is %d\n", rc);
            exit(-1);
        }
    }

    pthread_attr_destroy(&attr);

    for(int i=0; i<NUM_OF_THREADS; i++){
        pthread_join(threads[i], &status);
    }
    printf("\nThe Totals for all elements of the arrays are shown below\n");
    printf("using mutex, with semaphores, and without the semaphores   \n");
    printf("-----------------------------------------------------------\n\n");
    printf("The total of all elements in the array is: %d\n", total);
    printf("The total of all elements in the array using a semaphore is: %d\n", semaphoreTotal);
    printf("The total of all elements in the array using a mutex is: %d\n", mutexTotal);
   
    pthread_exit(NULL);
    return 0;

} 

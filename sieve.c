//
// Created by Christian Rhodes on 4/9/22.
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <math.h>
#include <time.h>

#define NUM_THREADS 4
#define M 10000 //M is the size of the range to be evaluated

typedef struct {
    int lowerBound;
    int upperBound;
    int s; //the first integer larger than the square root of the upper bound
    char *array;
    pthread_mutex_t *mutex;
} SieveInfo;

int sieve(void *param, pthread_mutex_t mutex) {
    SieveInfo *sinfo = (SieveInfo *) param;
    int done = 0;
    int mynum = sinfo->lowerBound;
    while ( ! done ) {
        //lock mutex
        pthread_mutex_lock(&mutex);

        //look for next prime
        while (sinfo->array[mynum] <= 0 && mynum < sinfo->upperBound) {
            mynum++;
        }
        if (mynum >= sinfo->upperBound) {
            done = 1;
        } else {
            //found next prime to cross off
            sinfo->array[mynum] = -1;
        }

        //unlock mutex
        pthread_mutex_unlock(&mutex);

        if (!done) {
            int i = mynum + mynum;
            while (i < sinfo->upperBound) {
                sinfo->array[i] = 0;
                i = i + mynum;
            }
        }
        pthread_exit(NULL);
    }
}

int main() {
    //clock to test compile time
    clock_t start = clock();

    //declare and initialize a mutex
    pthread_mutex_t mutex;
    pthread_mutex_init(&mutex, NULL);
    pthread_t tids[NUM_THREADS];

    //declare array and set all chars = 1
    char array[M];
    for (int i=0; i < M; i++) {
        array[i] = 1;
    }

    //declare struct
    SieveInfo sieveInfo;

    //set up info for sieveInfo
    sieveInfo.upperBound = M;
    sieveInfo.lowerBound = 2;
    sieveInfo.s = 1 + (int) sqrt((double) M);
    sieveInfo.array = array;
    sieveInfo.mutex = &mutex;

    //create threads and pass sieve info to them
    for (int i = 0; i < NUM_THREADS; ++i) {
        pthread_create(&tids[i], NULL, sieve, &sieveInfo);
    }
    for (int i = 0; i < NUM_THREADS; ++i) {
        pthread_join(tids[i], NULL);
    }

    //print primes
    printf("Prime Numbers: \n");
    for (int i = sieveInfo.lowerBound; i < M; i++) {
        if (sieveInfo.array[i] == 1 || sieveInfo.array[i] == -1) {
            printf("%d\n", i);
        }
    }

    //stop clock and print compile time
    clock_t stop = clock();
    double elapsed = (double) (stop - start) / CLOCKS_PER_SEC;
    printf("\nTime elapsed: %.5f\n", elapsed);

    return 0;
}
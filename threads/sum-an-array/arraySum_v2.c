#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define CACHE_LINE_SIZE 64

long long sum = 0;
pthread_mutex_t mutex;

typedef struct ThreadArg {
    long start_number;
    long end_number;
    char pad[CACHE_LINE_SIZE - (2*sizeof(long))];
} ThreadArg;

void makeArg(ThreadArg *ta, long start, long end) {
    ta->start_number = start;
    ta->end_number   = end;
}


void* threadFunc(void* arg) {
    ThreadArg *ta = (ThreadArg*) arg;
    long long thread_sum = 0;
    long n;

	for (n = ta->start_number; n <= ta->end_number; n++) {
		thread_sum += n;
	}
	
    pthread_mutex_lock(&mutex);
    sum += thread_sum;
    pthread_mutex_unlock(&mutex);
    pthread_exit(NULL);
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
            printf("usage: ./program <#threads>\n");
            exit(1);
    }

    if (atoi(argv[1]) <= 0)
            exit(1);

    int i, number_of_threads = atoi(argv[1]);	// Get number of threads to be created
    long n = 1000000000;				        // n numbers to sum
    pthread_t threads[number_of_threads];   	// Array to store the threads 
    ThreadArg args[number_of_threads];      	// Array to store the arguments that will be passed to the threads
    // clock_t sum_time;				        // Clock to measure thread sum time

    // Make arguments to be passed to threads and store that in args[] array
    long allocated_numbers = 1;
    for (i = 0; i < number_of_threads; i++) {
        if (i != number_of_threads-1) {
            makeArg(&args[i], allocated_numbers, (allocated_numbers+(n/number_of_threads)));
        }
        else {
            makeArg(&args[i], allocated_numbers, n);
        }
        allocated_numbers += n/number_of_threads + 1;
    }

    // Create the respected number of threads
    //sum_time = clock();
    for (i = 0; i < number_of_threads; i++) {
        pthread_create(&threads[i], NULL, threadFunc, &args[i]);
    }
    //sum_time = clock() - sum_time;

    // Wait for the threads to join
    for (i = 0; i < number_of_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    // Display sum and taken times
    //printf("for n = %ld, Sum = %lld\nThreads summing the array took %f seconds\n",
    //        n, sum, (((double)sum_time)/CLOCKS_PER_SEC));
	printf("for n = %ld, Sum = %lld\n", n, sum);

    return 0;
}


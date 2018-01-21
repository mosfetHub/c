#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define CACHE_LINE_SIZE 64

long *work_array;
long long sum = 0;
pthread_mutex_t mutex;

typedef struct ThreadArg {
	long start_index;
	long end_index;
	char pad[CACHE_LINE_SIZE - (2*sizeof(long))];
} ThreadArg;

void makeArg(ThreadArg *ta, long start, long end) {
	ta->start_index = start;
	ta->end_index   = end;
}


void* threadFunc(void* arg) {
	ThreadArg *ta = (ThreadArg*) arg;
	long long thread_sum = 0;
	long i;
	for (i = ta->start_index; i < ta->end_index; i++) {
		thread_sum += work_array[i];
	}
	pthread_mutex_lock(&mutex);
	sum += thread_sum;
	pthread_mutex_unlock(&mutex);
	pthread_exit(NULL);
}

int main(int argc, char* argv[]) {
	if (argc != 2) {
		printf("usage: <program> <threads>\n");
		exit(1);
	}

	if (atoi(argv[1]) <= 0)
		exit(1);

	int number_of_threads = atoi(argv[1]);
	pthread_t threads[number_of_threads];
	ThreadArg args[number_of_threads];

	long work_array_size = 1000000000; 
	work_array = (long*) calloc(sizeof(long), work_array_size);
	long i;
	for (i = 0; i < work_array_size; i++) {
		work_array[i] = i+1; 
	}

	long allocated_indices = 0;
	for (i = 0; i < number_of_threads; i++) {
		if (i != number_of_threads-1) {
			makeArg(&args[i], allocated_indices, (allocated_indices+(work_array_size/number_of_threads)));
		}
		else {
			makeArg(&args[i], allocated_indices, work_array_size);
		}
		allocated_indices += work_array_size/number_of_threads;
	}

	for (i = 0; i < number_of_threads; i++) {
		pthread_create(&threads[i], NULL, threadFunc, &args[i]);
	}

	for (i = 0; i < number_of_threads; i++) {
		pthread_join(threads[i], NULL);
	}

	printf("Sum = %lld\n", sum);

	free(work_array);	
	return 0;
}

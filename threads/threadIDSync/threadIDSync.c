#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define TOTAL_THREADS    4
#define RUN_UNTIL_OBTAIN 10

int CurrentID = 1;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

// Structure to use as the thread argument
typedef struct ThreadArg {
	long tid;		// Thread ID
	long obtained_turns;	// Count how many turns the thread obtained
	long waited_turns;	// Count how many turns the thread waited
} ThreadArg;

void init_arg(ThreadArg *ta, long tid) {
	ta->tid = tid;
	ta->obtained_turns = 0;
	ta->waited_turns   = 0;
}


// Thread function
void *run_thread(void *arg)
{
	ThreadArg *ta = (ThreadArg*) arg;
	
	int i;
	while (ta->obtained_turns < RUN_UNTIL_OBTAIN) {
		// Lock mutex
		pthread_mutex_lock(&mutex);
		
		// Check whether CurrentID equals the thread's ID or not 
		if (CurrentID == ta->tid) {
			printf("My Turn! ThreadID: %ld\n", ta->tid);
			ta->obtained_turns++;		// Increment obtained turns by the thread
			CurrentID++;			// Increment CurrentID
			if (CurrentID == 5) {
				CurrentID = 1;
			}
		}
		else {
			printf("Not My Turn! ThreadID: %ld\n", ta->tid);
			ta->waited_turns++;		// Increment turns the thread has waited
		}
		
		// Unlock the mutex
		pthread_mutex_unlock(&mutex);
	}

	// Exit
	pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
	pthread_t threads[TOTAL_THREADS];	// Threads
	ThreadArg args[TOTAL_THREADS];		// Arguments passed in to the threads
	
	// Create threads
	int  status;
	long t;
	for(t = 1; t <= TOTAL_THREADS; t++){
   		init_arg(&args[t-1], t);
		status = pthread_create(&threads[t-1], NULL, run_thread, &args[t-1]);
   		if (status) {
      			printf("ERROR; return code from pthread_create() is %d\n", status);
      			exit(-1);
   		}
	}

	// Wait for the threads to join
	for (t = 0; t < TOTAL_THREADS; t++) {
		pthread_join(threads[t], NULL);
	}

	// Print how many turns the threads waited
	printf("\n\n");
	for (t = 0; t < TOTAL_THREADS; t++) {
		printf("Thread #%ld: obtained turns = %ld, waited_turns = %ld\n", 
			t+1, args[t].obtained_turns, args[t].waited_turns);
	}

	return 0;
}

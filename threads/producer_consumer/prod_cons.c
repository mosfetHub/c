#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>
#include <pthread.h>

#define QUEUE_SIZE      5
#define TOTAL_PRODUCER  3
#define TOTAL_CONSUMER  15
#define TO_BE_PRODUCED  10
#define TO_BE_CONSUMED  10


//--------------------------------------------------------------------
// Queue Data Structure
// Implemented using circular arrays
//--------------------------------------------------------------------

typedef struct Queue {
	int head, tail, size, capacity;
	int* queue_array;
} Queue;

Queue* make_queue(int cap) {
	Queue* q       = (Queue*) malloc(sizeof(Queue));
	q->head        = 0;
	q->tail        = cap-1;
	q->size        = 0;
	q->capacity    = cap;
	q->queue_array = (int*) calloc(cap, sizeof(int));
	return q;
}

int is_full(Queue* q) {
	return (q->size == q->capacity);
}

int is_empty(Queue* q) {
	return (q->size == 0);
}

void enqueue(Queue *q, int data) {
	if (is_full(q)) { 
		return;
	}

	q->tail = (q->tail+1) % (q->capacity);
	q->queue_array[q->tail] = data;
	q->size = q->size + 1;
}

int dequeue(Queue *q) {
	if (is_empty(q)) {
		return INT_MIN;
	}

	int dequeued_data = q->queue_array[q->head];
	q->head = (q->head + 1) % (q->capacity);
	q->size = q->size - 1;
	return dequeued_data;
}

void free_queue(Queue *q) {
	free(q->queue_array);
	free(q);
}
//--------------------------------------------------------------------


// Global Variables
pthread_mutex_t mutex;
pthread_cond_t producer_cond; 
pthread_cond_t consumer_cond;

Queue *queue;
int total_produced;					// Total items produced
int total_consumed;					// Total items consumed
int waiting_producers = 0;
int waiting_consumers = 0;
int producer_wait_count[TOTAL_PRODUCER];		// Count how many times producers waited
int consumer_wait_count[TOTAL_CONSUMER];		// Count how many times consumrs waited


// Thread function run by the producer
void* producer(void* arg) {
	int producerID = (intptr_t) arg;
	int i = 1;
	while (total_produced < TO_BE_PRODUCED) {
		// Lock mutex
		pthread_mutex_lock(&mutex);
		
		//If the queue if full, no items to produce: wait
		if (is_full(queue)) {
			waiting_producers++;
			producer_wait_count[producerID-1]++;
			printf("PRODUCER #%d IS WAITING...\n", producerID);

			// Wait until the queue is full or the production target is reached
			while (is_full(queue) && total_produced < TO_BE_PRODUCED) {
				pthread_cond_wait(&producer_cond, &mutex);
			}
			waiting_producers--;
		}
		
		// If the production target was reached while waiting, break
		if (total_produced >= TO_BE_PRODUCED) {
			pthread_mutex_unlock(&mutex);
			break;
		}
		
		// Produce item
		printf("Producer #%d is producing its #%d item. ", producerID, i);
		enqueue(queue, 1);
		total_produced++;
		i++;
		printf("Total produced items = %d\n", total_produced);

		// Signal any waiting consumer that an item was produced
		pthread_cond_signal(&consumer_cond);

		// If produce item target is reached, then signal other producers to so that they end waiting
		if (total_produced == TO_BE_PRODUCED) {
			printf("Production target reached. Broadcasting to producers\n");
			pthread_cond_broadcast(&producer_cond);
		}

		// Unlock mutex
		pthread_mutex_unlock(&mutex);
	}

	// EXIT
	printf("Producer #%d Exiting\n", producerID);
	pthread_exit(NULL);
}


// Thread function run by the consumer
void* consumer(void* arg) {
	int consumerID = (intptr_t) arg;
	
	int i = 1;
	while (total_consumed < TO_BE_CONSUMED) {
		// Lock mutex
		pthread_mutex_lock(&mutex);
		
		// If the queue is empty, no items to consume: wait 
		if (is_empty(queue) && total_consumed < TO_BE_CONSUMED) {
			waiting_consumers++;
			consumer_wait_count[consumerID-1]++;
			printf("CONSUMER #%d WAITING...\n", consumerID);
			
			// Wait until the queue is empty or the consumption target is reached
			while (is_empty(queue) && total_consumed < TO_BE_CONSUMED) {
				pthread_cond_wait(&consumer_cond, &mutex);
			}
			waiting_consumers--;
		}

		// If the consumption target was reached while waiting, unlock and break
		if (total_consumed >= TO_BE_CONSUMED) {
			pthread_mutex_unlock(&mutex);
			break;
		}

		// All set to Consume item
		printf("Consumer #%d is consuming its #%d item. ", consumerID, i);
		dequeue(queue);
		total_consumed++;
		i++;
		printf("Total consumed = %d\n", total_consumed);

		// Signal any waiting producer that an item was consumed
		pthread_cond_signal(&producer_cond);

		// If the consumed item target is reached, then signal other consumers to exit waiting
		if (total_consumed == TO_BE_CONSUMED) {
			printf("Consumption target reached. Broadcasting to consumers\n");
			pthread_cond_broadcast(&consumer_cond);
		}
		
		// Unlock mutex
		pthread_mutex_unlock(&mutex);
	}
	
	// EXIT
	printf("Consumer #%d Exiting\n", consumerID);
	pthread_exit(NULL);
}

void initialize_all(void) {
	pthread_mutex_init(&mutex, NULL);
    	pthread_cond_init(&producer_cond, NULL);
    	pthread_cond_init(&consumer_cond, NULL);

    	queue = make_queue(QUEUE_SIZE);
    	total_produced   = 0;
    	total_consumed   = 0;
	
	int i;
    	for (i = 0; i < TOTAL_PRODUCER; i++) {
        	producer_wait_count[i] = 0;
    	}
    	for (i = 0; i < TOTAL_CONSUMER; i++) {
        	consumer_wait_count[i] = 0;
    	}
}

int main(int argc, char* argv[]) {
	// Threads
	pthread_t pt[TOTAL_PRODUCER];	// Producer threads
	pthread_t ct[TOTAL_CONSUMER];	// Consumer threads

	// Initialize global variables
	initialize_all();

	// Create threads
	int i, status;
	for (i = 0; i < TOTAL_CONSUMER; i++) {
		status = pthread_create(&ct[i], NULL, consumer, (void*)(intptr_t)(i+1));
		if (status) {
			exit(-1);
		}
	}
	for (i = 0; i < TOTAL_PRODUCER; i++) {
		status = pthread_create(&pt[i], NULL, producer, (void*)(intptr_t)(i+1));
		if (status) {
			exit(-1);
		}
	}

	// Wait threads to join back
	for (i = 0; i < TOTAL_CONSUMER; i++) {
		status = pthread_join(ct[i], NULL);
		if (status) {
			printf("FROM MAIN: ERROR JOINING CONSUMER. status = %d\n", status);
			perror("pthread_join() error");
			exit(-1);
		}
		else {
			printf("FROM MAIN: CONSUMER #%d JOINED\n", i+1);
		}
	}
	for (i = 0; i < TOTAL_PRODUCER; i++) {
		status = pthread_join(pt[i], NULL);
		if (status) {
			printf("FROM MAIN: ERROR JOINING PRODUCER. status = %d\n", status);
			perror("pthread_join() error");
			exit(-1);
		}
		else {
			printf("FROM MAIN: PRODUCER #%d JOINED\n", i+1);

		}
	}
	

	// Print how many times producer and consumers waited
	printf("\n\n");
	for (i = 0; i < TOTAL_PRODUCER; i++) {
		printf("Producer #%d waited %d times\n", i+1, producer_wait_count[i]);
	}
	for (i = 0; i < TOTAL_CONSUMER; i++) {
		printf("Consumer #%d waited %d times\n", i+1, consumer_wait_count[i]);
	}

	// Free queue and end
	free_queue(queue);
	return 0;
}


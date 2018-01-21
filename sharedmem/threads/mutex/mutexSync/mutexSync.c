#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#define NUM_LOOPS 500000000

long long sum = 0;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

// Function that'll be run by the thread
void* counting_thread (void* arg) {
        int offset = *((int*) arg);
        for (int i = 0; i < NUM_LOOPS; i++) {
                // Start Critical Section
                pthread_mutex_lock(&mutex);

                sum += offset;

                // End Critical Section
                pthread_mutex_unlock(&mutex);
        }
        pthread_exit(NULL);
}

int main (int argc, char* argv[]) {
        // Launcing Threads
        int offset_1 = 1;
        pthread_t threadID_1;
        pthread_create(&threadID_1, NULL, counting_thread, &offset_1);

        int offset_2 = -1;
        pthread_t threadID_2;
        pthread_create(&threadID_2, NULL, counting_thread, &offset_2);



        // Main thread waits for the background threads to join
        pthread_join(threadID_1, NULL);
        pthread_join(threadID_2, NULL);


        // Print result
        printf("Sum = %lld\n", sum);
        return 0;

}

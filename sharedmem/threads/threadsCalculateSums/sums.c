include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <pthread.h>

typedef struct sum_runner_struct {
        long long limit;
        long long answer;
} Runner;

// Thread Function to Generate Sum of 0 to N
void* sum_runner(void* arg) {
        Runner *arg_struct = (Runner*) arg;

        long long sum = 0;
        for (int i = 0; i <= arg_struct->limit; i++)
                sum += i;

        arg_struct->answer = sum;

        pthread_exit(0);
}

int main(int argc, char* argv[]) {
        if (argc < 2) {
                printf("Usage: %s <num1> <num2> ... <numn>\n", argv[0]);
                exit -1;
        }

        // Number of arguments
        int num_args = argc - 1;

        // sum_runner_struct
        Runner args[num_args];

        // Thread ID
        pthread_t tids[num_args];

        for (int i = 0; i < num_args; i++) {
                // Get input argument limit
                args[i].limit = atoll(argv[i+1]);

                // Create attributes
                pthread_attr_t attr;
                pthread_attr_init(&attr);

                pthread_create(&tids[i], &attr, sum_runner, &args[i]);
        }

        // Wait of all the threads to be done
        for (int i = 0; i < num_args; i++) {
                pthread_join(tids[i], NULL);
                printf("Sum until %s is answer is: %lld\n", argv[i+1], args[i].answer);
        }

        return 0;
}

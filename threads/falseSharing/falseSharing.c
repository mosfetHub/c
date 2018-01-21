#include <string.h>
#include <pthread.h>

#define CACHE_LINE_SIZE 64

typedef struct {
        int start;
        int end;
        double sum;
        char pad[CACHE_LINE_SIZE - (2*sizeof(int)) - (sizeof(double))];
} thr_data;

void init_data(thr_data *data, int s, int e) {
        data->start = s;
        data->end   = e;
        data->sum   = 0.0;
}

void* threadApprox(void* arg) {
        thr_data *x = (thr_data*) arg;

        int i;
        for (i = x->start; i < x->end; i++) {
                if (i%2 == 0)
                        x->sum += 1.0/(2*i+1);
                else
                        x->sum -= 1.0/(2*i+1);
        }

        pthread_exit(NULL);
}

int main(int argc, char* argv[]) {
        if (argc != 4) {
                printf("usage: <program> <start> <end> <number of threads>\n");
                exit(1);
        }
        int start     = atoi(argv[1]);
        int end       = atoi(argv[2]);
        int nbthreads = atoi(argv[3]);
        pthread_t threads[nbthreads];
        thr_data  args[nbthreads];
        
        int i;
        for (i = 0; i < nbthreads; i++) {
                init_data(&args[i], start, end);
        }

        for (i = 0; i < nbthreads; i++) {
                pthread_create(&threads[i], NULL, threadApprox, &args[i]);
        }

        for (i = 0; i < nbthreads; i++) {
                pthread_join(threads[i], NULL);
        }

        return 0;
}



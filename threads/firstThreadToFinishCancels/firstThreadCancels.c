#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct Pst {
        int tid;
        int tid_index;
        pthread_t* tids;
        int num_threads;
        int sleep_amount;
        int result;
} Pst;

Pst makePst(int _tid, int _tid_index, pthread_t* _tids, int _num_threads, int _sleep_amount) {
        Pst st;
        st.tid = _tid;
        st.tid_index = _tid_index;
        st.tids = _tids;
        st.num_threads = _num_threads;
        st.sleep_amount = _sleep_amount;
        st.result = -1;
        return st;
}

void notify(void* _arg) {
        Pst *arg = (Pst*) _arg;
        printf("Thread %d is cancelled!!\n", arg->tid_index);
}

void* threadFunc(void* _arg) {
        Pst *arg = (Pst*) _arg;

        pthread_cleanup_push(notify, arg);

        printf("Thread Function for thread %d started...\n", arg->tid_index);
        /*pthread_testcancel();*/
        usleep(arg->sleep_amount);
        pthread_testcancel();
        printf("Thread Function for thread %d is finished!\n", arg->tid_index);
        int i;
        for (i = 0; i < arg->num_threads; i++) {
                if (i != arg->tid_index) {
                        printf("Thread %d is calling cancel on thread %d\n", arg->tid_index, i);
                        pthread_cancel(arg->tids[i]);
                }
        }

        /*pthread_cleanup_pop(0);*/
        printf("Thread %d is exiting.\n", arg->tid_index);
        arg->result = 1;
        pthread_cleanup_pop(0);
        pthread_exit(NULL);
}

int main(int argc, char* argv[]) {
        pthread_t *threads = (pthread_t*) calloc(sizeof(pthread_t), 3);
        Pst args[3];
        args[0] = makePst(threads[0], 0, threads, 3, 100000000);
        args[1] = makePst(threads[1], 1, threads, 3, 1000000);
        args[2] = makePst(threads[2], 2, threads, 3, 10000000);

        pthread_create(threads  , NULL, threadFunc, &args[0]);
        pthread_create(threads+1, NULL, threadFunc, &args[1]);
        pthread_create(threads+2, NULL, threadFunc, &args[2]);

        int i;
        for (i = 0; i < 3; i++) {
                pthread_join(threads[i], NULL);
        }

        printf("\n\nFrom main: return values: ");
        for (i = 0; i < 3; i++) {
                printf("%d ", args[i].result);
        }
        printf("\n");

        free(threads);
        return 0;
}

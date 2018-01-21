#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct Pst {
        int thread_index;
        int *arr;
        int arr_len;
} Pst;

Pst makePst(int index, int *_arr, int _arr_len) {
        Pst st;
        st.thread_index = index;
        st.arr     = _arr;
        st.arr_len = _arr_len;
        return st;
}


void* threadFunc1(void* _arg) {
        Pst *arg = (Pst*) _arg;

        /* Own Copy */
        int thread_index = arg->thread_index;
        int *arr    = arg->arr;
        int arr_len = arg->arr_len;

        printf("Thread %d is printing: ", thread_index);
        int i;
        for (i = 0; i < arr_len; i++) {
                printf("%d ", arr[i]);
        }
        printf("\n");

        printf("Thread %d is swapping...\n", thread_index);
        int temp = arr[0];
        arr[0] = arr[arr_len-1];
        arr[arr_len-1] = temp;

        printf("Thread %d finished.\n", thread_index);
        pthread_exit(NULL);
}


void* threadFunc2(void* _arg) {
        usleep(100000);

        Pst *arg = (Pst*) _arg;

        /* Own Copy */
        int thread_index = arg->thread_index;
        int *arr    = arg->arr;
        int arr_len = arg->arr_len;

        printf("Thread %d is printing: ", thread_index);
        int i;
        for (i = 0; i < arr_len; i++) {
                printf("%d ", arr[i]);
        }
        printf("\n");

        printf("Thread %d is swapping...\n", thread_index);
        int temp = arr[0];
        arr[0] = arr[arr_len-1];
        arr[arr_len-1] = temp;

        printf("Thread %d finished.\n", thread_index);
        pthread_exit(NULL);
}



int main(int argc, char* argv[]) {
        int len = 10;
        int or_arr[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

        pthread_t threads[2];

        Pst args[2];
        args[0] = makePst(0, or_arr, len);
        args[1] = makePst(1, or_arr, len);

        pthread_create(&threads[0], NULL, threadFunc1, &args[0]);
        pthread_create(&threads[1], NULL, threadFunc2, &args[1]);

        int i;
        for (i = 0; i < 2; i++) {
                pthread_join(threads[i], NULL);
        }

        usleep(10000);
        for (i = 0; i < len; i++) {
                printf("%d ", or_arr[i]);
        }
        printf("\n");

        return 0;
}

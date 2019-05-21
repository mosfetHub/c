#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

pthread_mutex_t mutex;

typedef struct St {
        int field;
} St;

St makeSt(int val) {
        St st;
        st.field = val;
        return st;
}

void* threadFunc(void* _arg) {
        St *arg = (St*) _arg;
        printf("Thread is changing arg with initial field %d\n", arg->field);
        arg->field++;
        printf("Thread has changed the field, now it's %d\n", arg->field);
        pthread_exit(NULL);
}

int main(int argc, char* argv[]) {
        pthread_t tids[2];

        //St args[2];
        //args[0] = makeSt(-1, 0);
        //args[1] = makeSt(-1, 1);
        St arg = makeSt(-1);

        pthread_create(&tids[0], NULL, threadFunc, &arg);
        pthread_create(&tids[1], NULL, threadFunc, &arg);

        pthread_join(tids[0], NULL);
        pthread_join(tids[1], NULL);

        printf("From main: field = %d\n", arg.field);

        return 0;
}

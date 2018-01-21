#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>

#define SHSIZE 100

int main(int argc, char* argv[]) {
        int shmid;
        key_t key;
        char *shm;
        char *s;

        key = 9876;

        shmid = shmget(key, SHSIZE, IPC_CREAT | 0666);
        if (shmid < 0) {
                perror("Error during shmget\n");
                exit(1);
        }

        shm = shmat(shmid, NULL, 0);
        if (shm == (char*) -1) {
                perror("Error during shmat\n");
                exit(1);
        }

        memcpy(shm, "Hello World", 11);

        s = shm;
        s += 11;
        *s = 0;
        while (*shm != '*')
                sleep(1);

        return 0;
}

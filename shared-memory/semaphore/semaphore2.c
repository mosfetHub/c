#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <sys/mman.h>
#include <semaphore.h>


int main() {
        const char* zone = "/memzone1";
        int md = shm_open(zone,O_RDWR,S_IRWXU);
        int* t = mmap(NULL,4096,PROT_READ|PROT_WRITE,MAP_SHARED|MAP_FILE,md,0);

        sem_t* s = sem_open("/sema2",O_EXCL);

        int i;
        for (i=0;i <1000000;i++) {
                sem_wait(s);
                t[0] = t[0] + 1;
                sem_post(s);
        }

        int final = t[0];

        close(md);

        shm_unlink(zone);
        munmap(t,4096);
        sem_close(s);

        printf("Final: %d\n",final);
        return 0;
}

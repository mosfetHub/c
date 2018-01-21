#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <dirent.h>



int main(int arc, char* argv[]) {
        int n, status;
        n = 10;
        printf("A: %d\n", n);

        if( fork() == 0 ) {
                n = 100;
                printf("From Child:  B = %d\n", n);
                //exit(0);
        }
        else {
                n = 59;
                printf("From Parent:  C = %d\n", n);
                n = 93;
                printf("From Parent:  E =  %d\n", n);

                wait( &status );

                n = 47;
                printf("From Parent:  F = %d\n", n);
        }

        printf("G = %d\n", n);
}

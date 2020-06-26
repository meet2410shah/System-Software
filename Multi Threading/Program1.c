/* 

To compile this program use the following command
$ gcc -pthread Program1.c -o Program1.o

To run this program use the following command
$ ./Program1.o

*/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

/* Thread Routine */
void *thread(void *vargp) {
    printf("Hello, world!\n");
    return NULL;
}

int main() {
    pthread_t tid;
    pthread_create(&tid, NULL, thread, NULL);
    pthread_join(tid, NULL);
    exit(0);
}
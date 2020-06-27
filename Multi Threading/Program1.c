/**
 * To Compile this Porogram use the following command
 * $ gcc -pthread Program1.c -o Program1.o
 * 
 * To run this Program use the following command
 * $ ./Program1.o
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

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
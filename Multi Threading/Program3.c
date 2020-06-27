/**
 * To Compile this Porogram use the following command
 * $ gcc -pthread Program3.c -o Program3.o
 * 
 * To run this Program use the following command
 * $ ./Program3.o
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

/* Thread Routine */
void *mythread(void *vargp)
{
  printf("Thread.a\n");
  printf("Thread.b\n");
  return NULL;
}

int main()
{
  pthread_t tid;
  printf("Main.a\n");
  pthread_create(&tid, NULL, mythread, NULL);
  printf("Main.b\n");
  printf("Main.c\n");
  pthread_join(tid, NULL);
  printf("Main.d\n");
  exit(0);
}
/* To compile this program use the following command */
/* $ gcc -pthread Program9.c -o Program9.o */

/* To run this program use the following command */
/* $ ./Program9.o */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

void *thr_fn(void *arg)
{
  printf("child thread id %u is starting \n", (unsigned int)pthread_self());
  printf("child thread id %u is calling exit\n", (unsigned int)pthread_self());
  exit(0);
}

int main(void)
{
  pthread_t tid;
  printf("Main thread %u is starting\n", (unsigned int)pthread_self());
  int err = pthread_create(&tid, NULL, thr_fn, NULL);
  sleep(1); // ensure tid thread calls exit before main thread is completed
  /* If thread executes exit(), process terminates and the following printf() will not get executed */
  printf("Main thread %u is finished\n", (unsigned int)pthread_self());
  exit(0);
}
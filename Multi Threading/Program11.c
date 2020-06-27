/* To compile this program use the following command */
/* $ gcc -pthread 11_thread_example3.c -o 11_thread_example3.o */

/* To run this program use the following command */
/* $ ./11_thread_example3.o */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

void *thr_fn(void *arg)
{
  printf("child thread id %u is starting \n", (unsigned int)pthread_self());
  sleep(1); // ensure main thread exit first
  printf("child thread id %u is calling exit\n", (unsigned int)pthread_self());
  exit(0);
}
int main(void)
{
  pthread_t tid;
  printf("Main thread %u is starting\n", (unsigned int)pthread_self());
  int err = pthread_create(&tid, NULL, thr_fn, NULL);
  sleep(1); // just enough wait for child thread to start
  printf("Main thread %u is finished but let the child thread continue\n", (unsigned int)pthread_self());
  pthread_exit(0);
}
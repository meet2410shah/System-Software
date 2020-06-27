/* To compile this program use the following command */
/* $ gcc -pthread 12_thread_sync_example4.c -o 12_thread_sync_example4.o */

/* You may get warnings due to type cast - but you could still run the program */

/* To compile this program suppressing warnings use the following command */
/* $ gcc -pthread -Wno-int-to-pointer-cast -Wno-pointer-to-int-cast 12_thread_sync_example4.c -o 12_thread_sync_example4.o */

/* In case you dont want to suppress warnings you need to make some minor changes to the code like in earlier program 6.c */

/* To run this program use the following command */
/* $ ./12_thread_sync_example4.o */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

void *thr_fn1(void *arg)
{
  sleep(0.5);
  printf("child thread %u exiting\n", (unsigned int)pthread_self());
  return ((void *)1);
}

void *thr_fn2(void *arg)
{
  sleep(0.25);
  printf("child thread %u exiting\n", (unsigned int)pthread_self());
  pthread_exit((void *)2);
}

int main(void)
{
  int err;
  pthread_t tid1, tid2;
  void *tret;

  printf("Main thread is %u \n", (unsigned int)pthread_self());
  err = pthread_create(&tid1, NULL, thr_fn1, NULL);

  if (err != 0)
    printf("cannot create thread-1: %s\n", strerror(err));
  else
    printf("child thread %u is created\n", (unsigned int)tid1);

  err = pthread_create(&tid2, NULL, thr_fn2, NULL);

  if (err != 0)
    printf("cannot create thread-2: %s\n", strerror(err));
  else
    printf("child thread %u is created\n", (unsigned int)tid2);

  printf("main thread %u will wait for child thread %u\n", (unsigned int)pthread_self(), (unsigned int)tid1);
  err = pthread_join(tid1, &tret);

  if (err != 0)
    printf("cannot join with thread-1: %s\n", strerror(err));

  printf("child thread %u exit code %d\n", (unsigned int)tid1, (int)tret);
  printf("main thread %u will wait for child thread %u\n", (unsigned int)pthread_self(), (unsigned int)tid2);
  err = pthread_join(tid2, &tret);

  if (err != 0)
    printf("cannot join with thread-2: %s\n", strerror(err));

  printf("child thread %u exit code %d\n", (unsigned int)tid2, (int)tret);
  printf("Main thread %u exiting\n", (unsigned int)pthread_self());
  return 0;
}
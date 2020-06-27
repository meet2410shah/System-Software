/* To compile this program use the following command */
/* $ gcc -pthread 15_thread_racecond_with_mutex_example7.c -o 15_thread_racecond_with_mutex_example7.o */

/* To run this program use the following command */
/* $ ./15_thread_racecond_with_mutex_example7.o */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

pthread_mutex_t mymutex;

void *charatatime(void *str)
{
  char *ptr;
  int c;
  setbuf(stdout, NULL);
  pthread_mutex_lock(&mymutex);
  for (ptr = (char *)str; c = *ptr++;)
    putc(c, stdout);

  pthread_mutex_unlock(&mymutex);
}

int main()
{
  int ret;
  pthread_t thread1, thread2;
  ret = pthread_mutex_init(&mymutex, NULL);
  pthread_create(&thread1, NULL, charatatime, "1234567890123456789012345678901234567890");
  pthread_create(&thread2, NULL, charatatime, "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz");
  pthread_join(thread1, NULL);
  pthread_join(thread2, NULL);
  ret = pthread_mutex_destroy(&mymutex);
  exit(0);
}

/*
Race condition output i.e. without MUTEX

$ ./thread_racecond_with_mutex_example7.o 
12345678901abcde23fgh456789012345ij6789012345678kl90mnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz

$ ./thread_racecond_with_mutex_example7.o 
1234567890123456789012345678901234567890abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz
OR
$ ./thread_racecond_with_mutex_example7.o 
abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz1234567890123456789012345678901234567890
*/
/* To compile this program use the following command */
/* $ gcc -pthread Program14.c -o Program14.o */

/* To run this program use the following command */
/* $ ./Program14.o */

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
  for (ptr = (char *)str; c = *ptr++;)
    putc(c, stdout);
}

int main()
{
  pthread_t thread1, thread2;
  pthread_create(&thread1, NULL, charatatime, "1234567890123456789012345678901234567890");
  pthread_create(&thread2, NULL, charatatime, "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz");
  pthread_join(thread1, NULL);
  pthread_join(thread2, NULL);
  exit(0);
}

/* To compile this program use the following command */
/* $ gcc -pthread 13_thread_coop_example5.c -o 13_thread_coop_example5.o */

/* To run this program use the following command */
/* $ ./13_thread_coop_example5.o */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

void *print_count(void *ptr);
void *increment_count(void *ptr);

int NUM = 5;
int counter = 0;

int main()
{
  pthread_t thread1, thread2;
  pthread_create(&thread1, NULL, increment_count, NULL);
  pthread_create(&thread2, NULL, print_count, NULL);
  pthread_join(thread1, NULL);
  pthread_join(thread2, NULL);
  exit(0);
}

void *print_count(void *ptr)
{
  int i;
  for (i = 0; i < NUM; i++)
  {
    printf("counter = %d \n", counter);
    sleep(1);
  }
  pthread_exit(0);
}

void *increment_count(void *ptr)
{
  int i;
  for (i = 0; i < NUM; i++)
  {
    counter++;
    sleep(1);
  }
  pthread_exit(0);
}

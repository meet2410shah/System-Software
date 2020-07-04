/**
 * To Compile this Porogram use the following command
 * $ gcc -pthread Program8.c -o Program8.o
 * 
 * To run this Program use the following command
 * $ ./Program8.o
*/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

void *get_rand_num(void *args)
{
  int *nump = malloc(sizeof(int));
  srand(pthread_self());
  *nump = rand();
  printf("Random number from thread: %d\n", *nump);
  return nump;
}

int main()
{
  pthread_t tid;
  void *ptr = NULL;
  pthread_create(&tid, NULL, get_rand_num, NULL);
  pthread_join(tid, &ptr);
  printf("Random number from main thread: %d\n", *(int *)ptr);
  return 0;
}
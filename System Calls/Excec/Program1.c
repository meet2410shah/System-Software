#include <stdio.h>
#include <unistd.h>         // execxx()
#include <stdlib.h>         // exit()

int main() 
{
    fork();
    printf("0");
    if (fork() == 0) {
       execl("/bin/echo", "echo", "1", NULL);
       // Everthing After Exec will never execute!!!
       fork();
       printf("2");
    }
    printf("3");
    return 0;
}
#include <stdio.h>
#include <unistd.h>         // fork()
#include <sys/types.h>      // pid_t
#include <stdlib.h>         // exit()
#include <sys/wait.h>       // wait and waitpid()

void main() {
    pid_t child_pid;
    if (fork() == 0) {
        exit(0);                /* Terminate Child */
    } else {
        child_pid = wait(NULL);  /* Reaping Parent */
    }
    printf("Parent pid = %d\n", getpid());
    printf("Child pid = %d\n", child_pid);
}
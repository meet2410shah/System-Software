#include <stdio.h>
#include <unistd.h>         // fork()
#include <sys/types.h>      // pid_t
#include <stdlib.h>         // exit()
#include <sys/wait.h>       // wait and waitpid()

int main() {
    if (fork() == 0) {
        printf("Child : Hello From Child\n");
    } else {
        printf("Parent : Hello From Parent\n");
        wait(NULL);
        printf("Parent : Child has Terminated\n");
    }
    printf("Bye\n");
}
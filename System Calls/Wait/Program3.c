#include <stdio.h>
#include <unistd.h>         // fork()
#include <sys/types.h>      // pid_t
#include <stdlib.h>         // exit()
#include <sys/wait.h>       // wait and waitpid()

int main() {
    int status;
    if (fork() == 0) {
        exit(1);
    } else {
        wait(&status);
        if(WIFEXITED(status)) {
            printf("Exit status: %d\n", WEXITSTATUS(status));
        } else if (WIFSIGNALED(status)) {
            psignal(WTERMSIG(status), "Exit signal");   
        }
    }
}
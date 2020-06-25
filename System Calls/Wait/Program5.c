#include <stdio.h>
#include <unistd.h>         // fork()
#include <sys/types.h>      // pid_t
#include <stdlib.h>         // exit()
#include <sys/wait.h>       // wait and waitpid()

int main() {
    int i, status;
    pid_t pid[5];
    for (i = 0; i < 5; i++) {
        if ((pid[i] = fork()) == 0) {
            sleep(1);
            exit(100+i);
        }
    }
    for (i = 0; i < 5; i++) {
        pid_t child_pid = waitpid(pid[i], &status, 0);
        if (WIFEXITED(status)) {
            printf("Child %d terminated with status: %d\n", child_pid, WEXITSTATUS(status));
        }
    }
}
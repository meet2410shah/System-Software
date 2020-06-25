# Wait - Introduction
### Why does a process die?
A process terminates for one of 3 reasons:
  - It calls exit()
  - It returns (an int) from main
  - It receives a signal (from the OS or another process) whose default action is to terminate

### Key Observations!
  - the dying process produces status information.
  - Who looks at this? The parent process!

### System Call : Exit()
```c
void exit(int status);
```
  - Terminates a process with a specified status
  - By convention, status of 0 is normal exit, non-zero indicates an error of some kind.
```c
void foo() {
    exit(1);  /* No return */
}
int main() {
    foo(); 
    return 0; /* No return */
}
```

### Reaping Children
  - wait(): parents reap their dead children
    - Given info about why child died, exit status, etc.
  - Two variants
    - wait()    : wait for and reap next child to exit
    - waitpid() : wait for and reap specific child

### System Call : Wait()
```c
pid_t wait(int *stat_loc);
```
When called by a process with one or more children:
- Waits (if needed) for a child to terminate
- Reaps a terminated child (if one or more terminated children, arbitrarily
pick one)
- Returns reaped child’s pid and exit status info via pointer (if
non-NULL)

When called by a process with no children:
- Return -1 immediately

### Example Program 1
```c
int main() {
    pid_t child_pid;
    if (fork() == 0) {
        exit(0);                /* Terminate Child */
    } else {
        child_pid = wait(NULL);  /* Reaping Parent */
    }
    printf(“Parent pid = %d\n”, getpid());
    printf(“Child pid = %d\n”, child_pid);
}
```

### Example Program 2
```c
int main() {
    if (fork() == 0) {
        printf("Child : Hello From Child\n");
    } else {
        printf("Parent : Hello From Parent");
        wait(NULL);
        printf("Parent : Child has Terminated");
    }
    printf("Bye\n");
}
```

### Child Status Information
Status information about the child reported by wait is more than just the exit status of the child.
- Normal / Abnormal Termination
- Termination Cause
- Exit Status

### WIF... Macros
1. WIFEXITED(status) : child exited normally.
    - WEXITSTATUS(status) : return code when child exits.
2. WIFSIGNALED(status) : child exited because of a signal was not caught.
    - WTERMSIG(status) : Gives the number of the terminating signal.
3. WIFSTOPPED(status) : child is stopped.
    - WSTOPPED(status) : Gives the number of the stop signal.

### Prints Information About a Signal
```c
void psignal(unsigned sig, const char *s);     
```
### Example Program 3
```c
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
```
### Example Program 4
If Multiple Children Completed, will reap in Arbitrary Order
```c
int i, status;
pid_t pid[5];
for(i = 0; i < 5; i++) {
    if ((pid[i] = fork()) == 0) {
        sleep(1);
        exit(100 + i);
    }
}
for(i = 0; i < 5; i++) {
    pid_t cpid = wait(&stat);
    if (WIFEXITED(status)) {
        printf(“Child %d terminated with status: %d\n”, cpid, WEXITSTATUS(status));
    }
}
```












###
###
###
###


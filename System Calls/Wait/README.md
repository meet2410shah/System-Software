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
pid_t pid_result = wait(int *status_pointer);
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
    pid_t cpid = wait(&status);
    if (WIFEXITED(status)) {
        printf("Child %d terminated with status: %d\n", cpid, WEXITSTATUS(status));
    }
}
```

### System Call : Waitpid()
- Waiting for a specific process.
- When parent has more than one child, or you want to check for exited child but not block
```c
pid_t pid_result = waitpid(child_pid, &status, options);
```
- child_pid :
    -   -1 means any child
    -   pid of Specific child
- status : Same as wait call.
- options : 
    - 0 = no options, wait until child exits
    - WNOHANG = don't wait, just check
- Return Value:
    - pid of child, if child has exited
    - 0, if using WNOHANG and child hasn't exited.

### Example Program 5
Can use waitpid() to reap children in order.
```c
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
        printf(“Child %d terminated with status: %d\n”, child_pid, WEXITSTATUS(status));
    }
}
```

### Zombie Process
What should happen if dead child processes are never reaped? That is, the parent has not waited() on them.
1. The OS should remove them from the process table.
2. The OS should leave them in the process table.
3. Do nothing.

**Zombie Process** : A process that has terminated but not been
reaped by its parent (Also known as defunct process).

**Dead Process** are still tracked by OS
- Parent may still reap them, want to know status.
- Don't want to re-use the Process ID yet.

### Reaping children
- Parents are responsible for reaping their children
- What should happen if parent terminates without reaping its children?
- Who reaps the children?

**Orphaned Processes** : A process that has not been reaped by its
terminated parent.
- Orphaned processes are adopted by the OS kernel
- ... and the kernel always reaps its children.
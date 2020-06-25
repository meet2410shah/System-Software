# Exec : Family of System Calls - Introduction
- When fork() creates a child process with a copy of same code, data etc as parent process but if you need to run another process as child process then?
- A process may replace its current code, data, and stack with those of another executable by using one of the “exec()” family of system calls.
- When a process executes an “exec()” system call, its PID and PPID numbers stay the same only the code that the process is executing changes.

### System Call:
There are several different flavours of the exec() call, but they all perform essentially the same task.
The following is a list of all six exec() variants and the parameter numbers and types they take:
```c 
int execl(pathname, argo, ..., argn, 0);
```
```c
int execv(pathname, argv);
```
```c
int execlp(cmdname, arg0, ..., argn, 0);
```
```c
int execvp(cmdname, argv);
```
```c
int execle(pathname, arg0, ..., arga, 0, envp);
```
```c
int execve(pathname, argv, envp);
```

### Example Program 1
```c
int main() 
{
    fork();
    printf("0");
    if (fork() == 0) {
       execl("/bin/echo", "echo", "1", NULL);
       fork();
       printf("2");
    }
    printf("3");
    return 0;
}
```

### Example Program 2

```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
   int i;
   for (i = 1; i < argc; i++) {
      execvp(argv[i], &(argv[i]));
    }
   return 0;
}
```

Write the following command to run this code:
```sh
$ gcc progrm2.c -o program.o
$ ./program2.o ls program2 program2 program2 program2
```

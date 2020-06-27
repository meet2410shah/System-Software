# Inter-Process Communications using Signals

### What is a Signal?
- Program must sometimes deal with unexpected or unpredictable events, such as :
    - A floating point error
    - A power failure
    - An alarm clock “ring”
    - The death of a child process
    - A termination request from a user ( i.e., Control + C )
    - A suspend request from a user ( i.e., Control + Z )
- These kind of events are sometimes called **Interrupts**, as they must interrupt the regular flow of a program in order to be processed.
- When UNIX recognizes that such an event has occurred, it sends the
corresponding process a signal.

### Signals

- A signal is a software generated interrupt that is sent to a process by the OS because:
    - It did something (oops)
    - The user did something (pressed ^C)
    - Another process wants to tell it something (SIGUSR?)
- There are a fixed set of signals that can be sent to a process.
- A software interrupt delivered to a process by the OS because:
    - It did something (segmentation fault, FPE)
    - The user did something (pressed Ctrl+C)
    - Another process wants to tell it something (SIGUSR?)
- One way a process can communicate with other processes

### Signal Types:
- Some signals are asynchronous, they may be raised at any time (user pressing Ctrl+C)
- Some signals are directly related to hardware (illegal instruction, arithmetic
exception, such as attempt to divide by 0) - synchronous signals

Signals are defined in “/usr/include/sys/signal.h” following the header
chain actual constants are defined in “/usr/include/bits/signum.h”

### Signal Numbers
- Signals are identified by integers.
- Signal numbers have symbolic names. For example, SIGCHLD is the
number of the signal to a parent when a child terminates.

```c
#define SIGHUP 1    /* Hangup. */
#define SIGINT 2    /* Interrupt. */
#define SIGQUIT 3   /* Quit. */ 
#define SIGILL 4    /* Illegal instruction. */
#define SIGTRAP 5   /* Trace trap. */
#define SIGABRT 6   /* Abort. */
```
### OS Structues for Signals
- For each process, the operating system maintains 2 integers with the bits corresponding to a signal numbers.
- The two integers keep track of:
    - pending signals
    - blocked signals
- With 32 bit integers, up to 32 different signals can be represented.
#### Example:
- In the example below, the SIGINT ( = 2) signal is blocked and no
signals are pending.
* **Pending Signals**
    |31|30|29| .... |03|02|01|00|
    | 0 | 0 | 0 | .... | 0 | 0 | 0 | 0 |
* **Blocked Signals**
    |31|30|29| .... |03|02|01|00|
    | 0 | 0 | 0 | .... | 0 | 1 | 0 | 0 |

### Sending, Receiving Signals
- A signal is sent to a process setting the corresponding bit in the
pending signals integer for the process.
- Each time the OS selects a process to be run on a processor, the
pending and blocked integers are checked.
- If no signals are pending, the process is restarted normally and
continues executing at its next instruction.
- If 1 or more signals are pending, but each one is blocked, the process
is also restarted normally but with the signals still marked as pending.
- If 1 or more signals are pending and NOT blocked, the OS executes
the routines in the process's code to handle the signals.

### Default Signal Handlers
There are several default signal handler routines.
- Each signal is associated with one of these default handler routine.
- The different default handler routines typically have one of the
following actions:
    - Ignore the signal i.e., do nothing, just return - IGN
    - Terminate the process - TERM
    - Unblock a stopped process - CONT
    - Block the process - STOP

### Actions When Signal is Recieved

The default handler usually performs one of the following actions:
- **Abort** : Terminate the process and generate a core file (dump)
- **Quit/Exit** : Terminate the process without generating a core image file (quit)
- **Ignore** : Ignore and discard the signal (ignore)
- **Stop** : Suspend the process (suspend)
- **Continue** : Resume the process (resume)


### User Defined Signal Handlers
- A process can replace the default signal handler for almost all signals
(but not SIGKILL and SIGSTOP) by its own handler function.
- A signal handler function can have any name, but must have return
type void and have one int parameter.
- Example, you might choose the name ***sigchld_handler*** for a signal
handler for the **SIGCHLD** signal (termination of a child process). Then
the declaration would be:
```c
void sigchld_handler(int sig);
```
- When a signal handler executes, the parameter passed to it is the
number of the signal.
- A programmer can use the same signal handler function to handle
several signals. In this case the handler would need to check the
parameter to see which signal was sent.
- On the other hand, if a signal handler function only handles one
signal, it isn't necessary to bother examining the parameter since it
will always be that signal number.

### Sending signals from keyboard
- Process suspended using Ctrl+Z (SIGSTSTP) can be brought back to life
using fg command which sends (SIGCONT) signal to resume
- For background process can't use Ctrl+C, Ctrl+Z etc. hence kill
command is used
- how one can run process in background ? Using bg command
- kill [options] pid
    * --l lists all signals you can send
    * --signal number
- kill command can send SIGKILL signal upon pressing Ctrl+C to the
foreground process which cannot be ignored.
- Most signals can be ignored except SIGKILL and SIGSTOP
- Application can trap the signal (e.g. SIGINT when Ctrl+C pressed) and handle signal disposition via signal handler
- Signal handler can perform default action (usually process termination) or customized actions such as blocking the signal.

### Example Program 1
Requesting an Alarm Signal SIGALRM using alarm() System Call
**System Call : Alarm()**
```c
unsigned int alarm (unsigned int count)
```
- alarm() instructs the kernel to send the SIGALRM signal to the calling
process after counting seconds.
- If an alarm had already been scheduled, that alarm is overwritten.
- If count is 0, any pending alarm requests are cancelled.
- The default handler for this signal displays the message “Alarm clock”
and terminates the process.
- alarm() returns the number of seconds that remain until the alarm
signal is sent

```c
#include <stdio.h>
#include <unistd.h>

int main(void) {
	alarm(3);
	printf("Looping forever\n");
	while(1);
	printf("This point is never reached");
}
```

### Handling Signals (Overriding Default Action)
**System Call** : 
```c
void(*signal(int sigCode, void (*func)(int))) (int)
```
- **signal()** allows a process to specify the action that it will take when a
particular signal is received.
- **sigCode** specifies the signal number (as per the table shown in earlier slides) that is to be reprogrammed
- **func** may be one of several values:
    - **SIG_IGN**, which indicates that the specified signal should be ignored and discarded
    - **SIG_DFL**, which indicates that the kernel’s default handler should be used.
    - An address of a user-defined function, which indicates that the function should be executed when the specified signal arrives.

### Handling Signals
- The valid signal numbers are stored in 
```sh
$ /usr/include/signal.h
```
or
```sh
$ /usr/include/bits/signum.h
```
- The signals SIGKILL and SIGSTP may not be reprogrammed.
- **signal()** returns the previous func value associated with sigCode if
successful; otherwise, it returns a value of -1
- **signal()** system call can be used to override the default action
- A child process inherits the signal settings from its parent during a fork()

### System Call : pause()
```c
int pause ( void )
```
- **pause()** suspends the calling process and returns when a calling
process receives a signal.
- It is most often used to wait efficiently for a signal.
- **pause()** doesn’t return anything useful.

### Example Program 2
```c
#include <stdio.h>
#include <signal.h>
#include <unistd.h>

int alarmFlag = 0;	                    

void alarmHandler() {
	printf("An alarm clock signal was received \n");
	alarmFlag=1;
}

int main(void) {
	signal(SIGALRM, alarmHandler);	    
	alarm(3);	                        
	printf("Looping \n");
	while(!alarmFlag) {	                
		pause();	                    
	} 
	printf("Loop ends due to alarm signal \n");
}
```

### Protecting Critical Code from Ctrl-C attack
- Overriding may be used to protect critical pieces of code against Control-C attacks and other such signals.
- It can be restored after the critical code has executed.
- Here’s the source code of a program that protects itself against SIGINT signals.

### Example Program 3

```c
#include <stdio.h>
#include <signal.h>
#include <unistd.h>

int main(void) {
	printf("I can be Control-C ed\n");
	sleep(3);
	printf("I am protected from Control-C now\n");
	signal(SIGINT, SIG_IGN);
    sleep(3);
	printf("I can be Control-C ed again\n");
	sleep(3);
	printf("Bye! \n");
}
```

### Process Groups and Control Terminals
- When you’re in a shell and you execute a program that creates several
children, a single Control-C from the keyboard will normally terminate
the program and its children and then return you to the shell.
- In order to support this kind of behavior, UNIX introduced a few new
concepts.
- In addition to having a unique process ID number, every process is also a
member of a process group.
    - Several processes can be members of the same process group.
    - When a process forks, the child inherits its process group from its parent.
    - A process may change its process group to a new value by using setpgid()
    - When a process execs, its process group remains the same.
- Every process can have an associated control terminal, which is typically the terminal where the process was started.
- When a process forks, the child inherits its control terminal from its parent.
- When a process execs, its control terminal stays the same.
- Every terminal can be associated with a single control process.
- When a metacharacter such as a `Control + C` is detected, the terminal sends the appropriate signal to all of the processes in the process group of its control process.
- If a process attempts to read from its control terminal and is not a member of the same process group as the terminal’s control process,
- The process is sent a SIGTTIN signal, which normally suspends the process.
- When an interactive shell begins, it is the control process of a
terminal and has that terminal as its control terminal.
- When a shell executes a foreground process:
    - The child shell places itself in a different process group before exec’ing the command and takes control of the terminal.
    - Any signals generated from the terminal thus go to the foreground command rather than to the original parent shell.
    - When the foreground command terminates, the original parent shell takes back control of the terminal
- When a shell executes a background process:
    - The child shell places itself in a different process group before executing, but does not take control of the terminal.
    - Any signals generated from the terminal continue to go to the shell.
    - If the background process tries to read from its control terminal, it is suspended by a SIGTTIN signal.

### System Call : setpgid() and getpgid()
```c
pid_t setpgid( pid_t pid, pid_t pgrpId)
```
- setpgid() sets the process-group ID of the process with PID pid to pgrpId.
    - If pid is zero, the caller’s process group ID is set to pgrpId.
    - In order for setpgid() to succeed and set the process group ID, at least one of the following conditions must be met:
        - The caller and the specified process must have the same owner.
        - The caller must be owned by a super-user.
- If setpgid() fails, it returns a value of -1.
```c
pid_t getpgid( pid_t pid )
```
- getpgid() returns the process group ID of the process with PID pid.
- If pid is zero, the process group ID of the caller is returned.

### Example Program 4
- Example illustrates the fact that a terminal distributes signals to all of
the processes in its control process’ process group.
- Since the child inherited its process group from its parent, both the parent and child caught the SIGINT signal.
```c
#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void signal_handler() {
	printf("process %d, %d got a SIGINT\n", getpid(), getpgid(getpid()));
}

int main(void) {
	signal(SIGINT, signal_handler);
	if (fork() == 0) {
	    printf("child pid %d and child group %d waits\n", getpid(), getpgid(0));
	} else {
		printf("parent pid %d and parent group %d waits\n", getpid(), getpgid(0));
    }
	pause();
}
```

### Example Program 5
- Example illustrates the fact that a terminal distributes signals to all of the processes in its control process’ process group.
- Since the child is in different process group SIGINT signal was caught only by parent and not child. We had to explicitly kill child since it was orphan.
```c
#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void signal_handler()
{
	printf("process %d got a SIGINT\n", getpid());
}

int main(void)
{
	signal(SIGINT, signal_handler);
	if (fork() ==0)	// child
	{
		setpgid(getpid(), 0);	// set process group as pid for child
		printf("child pid %d and child group %d waits\n", getpid(), getpgid(0));
	}
	else		// parent
		printf("parent pid %d and parent group %d waits\n", getpid(), getpgid(0));

	pause();	// wait for signal

}
```

The following is the command that you need to perform in order to kill the child process which is currently an orphan process.
```sh
$ ./a.out
parent pid 19460 and parent group 19460 waits
child pid 19461 and child group 19461 waits
^C
process 19460 got a SIGINT
$ ps 19461
PID TTY         STAT    TIME COMMAND
19461 pts/1     S       0:00 ./a.out
$ kill 19461
$ ps 19461
PID TTY         STAT    TIME COMMAND
```

### Sending Signal from another Process - System Call : kill()
- A process may send a signal to another process by using the kill() system call
- kill() is a misnomer, since many of the signals that it can send to do not terminate a process
- Its call kill() because it was originally designed to terminate a process
which is still one of its function
```c
int kill(pid_t pid, int sigCode)
```
- kill() sends the signal with value sigCode to the process with PID pid.
- kill() succeeds and the signal is sent as long as at least one of the following conditions is satisfied:
    - The sending process and the receiving process have the same owner.
    - The sending process is owned by a super-user.
- There are a few variations on the way that “kill()” works:
    - If pid is positive, then signal is sent to the process with the ID specified by pid
    - If the pid is negative, but not -1, then signal is sent to every process in the process group whose ID is -pid.
    - If pid is zero, then signal is sent to every process in the same process group as of the calling process.
    - If pid is -1 and the sender is owned by a super-user, the signal is sent to all processes, including the sender.
    - If pid is -1 and the sender is not owned by a super-user, the signal is sent to all of the processes owned by the same owner as that of the sender, excluding the sending process.

### Detecting death of a Child Process
- When a parent’s child terminates, the child process sends its parent a SIGCHLD signal.
- A parent process often installs a handler to deal with this signal; this handler typically executes a wait() system call to accept the child’s termination code and let the child dezombify.
- The parent can choose to ignore SIGCHLD signals, in which case the child
dezombifies automatically.
- The program works by performing the following steps:
    - The parent process installs a SIGCHLD handler that is executed when its child process terminates.
    - The parent process forks a child process to execute the command.
    - The parent process sleeps for the specified number of seconds. when it wakes up, it sends its child process a SIGINT signal to kill it.
    - If the child terminates before its parent finishes sleeping, the parent’s SIGCHLD handler is executed, causing the parent to terminate immediately.
```c
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>

int delay;
void childHandler();

int main(int argc, char* argv[]) {
    int pid;
    signal(SIGCHLD, childHandler);
    pid = fork();
    if(pid == 0) {
        execvp(argv[2], &argv[2]);
        perror("limit");          
    } else {                      
        sscanf(argv[1], "%d", &delay);
        sleep(delay);                 
        printf("Child %d exceeded limit and is being killed \n", pid);
        kill(pid, SIGINT);
    }
}

void childHandler() {
    int childPid, childStatus;
    childPid = wait(&childStatus);
    printf("Child %d terminated within %d seconds \n", childPid, delay);
}
```

```sh
$ ./a.out 4 ps
```

### Suspending and Resuming Process using SIGSTOP and SIGCONT signals
- The SIGSTOP and SIGCONT signals suspend and resume a process, respectively.
- They are used by the UNIX shells that support job control to implement built-in commands like stop, fg, and bg.
- The next Program performs these steps:
    - The main program creates two children; they both enter an infinite loop and display a message every second.
    - The main program waits for three seconds and then suspend the first child.
    - After another three seconds, the parent restarts the first child, waits a little while longer, and then terminated both children.
```c
#include <signal.h>
#include <stdio.h>
#include <unistd.h>

int main(void) { 
    int pid1, pid2;
    pid1 = fork();
    if (pid1 == 0) {            /* First child */
        while(1) {              /* Infinite loop */
            printf("pid1 is alive \n");	
            sleep(1);
        }
    }
    pid2 = fork();              /* Second child */
    if (pid2 == 0) {
        while(1) {              /* Infinite loop */
            printf("pid2 is alive \n");	
            sleep(1);
        }
    }
    sleep(3);
    kill(pid1, SIGSTOP);        /* Suspend First child */
    sleep(3); 
    kill(pid1, SIGCONT);        /* Resume First child */
    sleep(3);
    kill(pid1, SIGINT);         /* Kill First child */
    kill(pid2, SIGINT);         /* Kill second child */
}
```

### 







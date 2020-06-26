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

### Signal Handling 
- kill [options] pid
    * --l lists all signals you can send
    * --signal number
- kill command can send SIGKILL signal upon pressing Ctrl+C to the
foreground process which cannot be ignored.
- Most signals can be ignored except SIGKILL and SIGSTOP
- Application can trap the signal (e.g. SIGINT when Ctrl+C pressed) and handle signal disposition via signal handler
- Signal handler can perform default action (usually process termination) or customized actions such as blocking the signal.
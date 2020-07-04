# Threads
###### A new Abstraction of Program Execution

### Web Server
- A process listens for requests.
- When a new request comes in, create a new child process to handle this request.
- Multiple requests can be handled at the same time by different child processes.

```c
while(1) {
    int sock = accept();
    if (0 == fork()) {
        handle_request();
        close(sock);
        exit(0);
    }
}
```
##### Is this implementation is efficient?

### Web Server using fork() is inefficient
- **Time**
    - creating a new process for each incoming request
    - context switching between processes
- **Space**
    - each child process has its own copy of the address space, but they are almost the same
- **Inter-process communication (IPC)**
    - the child process may generate a response which will be used by the parent process
    - Extra overhead to make this happen (use a file/pipe/socket/shared memory)

### What is a Thread?
- A normal process is a running program with a single control flow, i.e., a single PC.
- A multi-threaded program has multiple control flows, i.e., multiple PCs.
    - One thread is a single control flow through a program

### Multi-threaded Process Address Space
- Multiple threads share the same address space
    - no need to copy address space when creating thread
    - no need to switch address space on context switch
- There are multiple stacks in the address space, one for each thread.

### Web Server Using Threads
- Different threads share and modify the global variable
```c
int global_counter = 0;
web_server() {
    while (1) {
        int sock = accept();
        thread_create(handle_request, sock);
    }
}
```
### The counter thing doesn’t work using fork(). Why?
```c
int global_counter = 0;
while (1) {
    int sock = accept();
    if (0 == fork()) {
        handle_request();
        ++global_counter;
        close(sock);
        exit(0);
    }
}
```
- counter is always 0 in the parent process, since changes only happen in children.
- If you really want to make this work using fork(), need inter-process communication.

### Conclusions
- Lighter weight
    - faster to create and destroy
    - faster context switch
- Sharing
    - threads can solve a single problem concurrently and can easily share code, heap and global variables

### Thread Programming
- Linux pthreads using kernel-level threads
- Thread prototypes: `#include <pthread.h>`
    -  More than 60 functions
- To compile:
```sh
$ gcc -lpthread Program.c -o Program
```
OR
```sh
$ gcc -pthread Program.c -o Program
```

### Basic operations on threads
- **Creating and starting a thread**
    - like an asynchronous procedure call.
- **Joining with a thread**
    - blocks the calling thread until a target thread completes.
    - a typical operation when a thread needs to use the return value of the target-thread’s starting procedure.

### Thread Creation
```c
int pthread_create (pthread_t *tid,
                pthread_attr_t *attr,
                void *(*func)(void *),
                void *arg);
```
- **func** : (input parameter) 
    - the name of the user-defined function to be executed by the newly created thread.
    - returns a void pointer, single void pointer argument
- **arg** : (input parameter)
    - the argument to be passed to the function **func**.
- Why void * as argument type?
- The argument type should:
    - Work for all types and sizes of arguments
    - Even work for different numbers of arguments
- **attr** : (input parameter)
    - pointer to structure specifying the thread’s attributes (e.g. stack size, etc.)
    - If set to NULL, the system defaults are used.
- **tid** : (output parameter)
    - pointer to location where the id of the newly created thread will be stored.

### Threaded “hello, world”
# Wrong Implementation!
```c
#include "csapp.h"
void *thread(void *vargp) {
    printf("Hello, world!\n");
    return NULL;
}
int main() {
    pthread_t tid;
    Pthread_create(&tid, NULL, thread, NULL);
    exit(0);
}
```
### Thread coordination
- Threads can only execute as long as their containing process exists
- In the example, what could happen if the thread routine has not reached printf with the main thread reaching the end of main()?
- We need to use join() to make the main thread wait for the other thread to finish.
```c
pthread_join()
```
```c
int pthread_join (pthread_t thread,
                void **value_ptr);
```
- **thread**: (input parameter)
    - id of the thread to wait on
- **value_ptr**: (output parameter) 
    - value given to
- **pthread_exit()** by the terminating thread (which happens to always be a void *)
- Type is void ** because type of location to be updated is void*

```c
#include "csapp.h"
void *thread(void *vargp) {
    printf("Hello, world!\n");
    return NULL;
}
int main() {
    pthread_t tid;
    pthread_create(&tid, NULL, thread, NULL);
    pthread_join(tid, NULL);
    exit(0);
}
```

Multi-Process Model  | Multi-Threaded Model
------------- | -------------
Process Spawning  | Thread Spawning
Process creation involve following actions: | Threads are created within and belonging to processes
Setting up process control block (PCB) | All the threads created within one process share the resources of the process including the address space
Allocation of memory address space | Scheduling is performed on a per-thread basis.
Loading program into allocated memory address space | The thread model is a finer grain scheduling model than the process model
Passing PCB to scheduler to queue up the process to run | Threads have a similar lifecycle as the processes and will be managed mainly in the same way as processes are

### Why use Multi-Threaded model over Multi-Process model?
- **A common terminology**:
    - Heavyweight Process = Process
    - Lightweight Process = Thread
- **Advantages (Thread vs. Process)**:
    - Much quicker to create a thread than a process
        - spawning a new thread only involves allocating a new stack and a new CPU state block
    - Much quicker to switch between threads than to switch between processes
    - Threads share data easily
- **Disadvantages (Thread vs. Process)**:
    - Processes are more flexible
        - They don’t have to run on the same processor
    - No security between threads: One thread can stomp on another thread's data
    - For threads which are supported by user thread package instead of the kernel:
        - If one thread blocks, all threads in task block.

### Thread Implementation
- Two broad categories of thread implementation
    1. User-Level Threads (ULT)
    2. Kernel-Level Threads (KLT)

### User-Level Threads (ULT)
- Kernel is not aware of existence of threads, it knows only processes with one thread of execution
- Each user process manages its own private thread table
- Advantages:
    - light thread switching: does not need kernel mode privileges
    - cross-platform: ULT can run on any underlying O/S
- Disadvantage:
    - if a thread blocks, the entire process is blocked, including all other threads in it

### Kernel-Level Thread (KLT)
- the kernel knows about and manages the threads: creating and destroying threads are system calls
- Advantage:
    - fine-grain scheduling, done on a thread basis
    - if a thread blocks, another one can be scheduled without blocking the whole process
- Disadvantage:
    - heavy thread switching involving mode switch

### POSIX Threads (pthreads) Library
- OS Support: FreeBSD, NetBSD, OpenBSD, Linux, Mac OS X, Android and Solaris. DR-DOS and Microsoft Windows implementation also exists
- Library : libpthread
- Header file : pthread.h
- More then 100 threads functions categorized in to 4 category
    - Thread Management
    - Mutexes
    - Condition Variables
    - Synchronization between Threads using read/write locks and barriers
- To compile with gcc you need to use –pthread option of gcc to link libpthread

### Thread Identification
```c
#include <pthread.h>
pthread_t pthread_self(void);
```
Returns thread ID of the calling thread.

**Note:** Each process has at least single thread running by default even if you have not created thread explicitly.

### Thread Creation
```c
#include <pthread.h>
int pthread_create(pthread_t *tidp, const pthread_attr_t *attr, void
*(*start_routine)(void*), void *arg);
```
Returns 0 on success and error number in case of failure
#### Parameters:
- **tidp** : pointer to thread ID variable which will be set with thread ID for newly created thread.
- **attr** : specific attributes to control the thread (default value to pass is NULL)
- **start_routine** : pointer to function which will be executed upon creation of a new thread
- **arg** : pointer to argument(s) to be passed to start_routine function. Note if more then one parameter need to be passed than you need to use structure instead of primitive type.

### Thread termination
- 4 possible ways
- **Implicit termination**: the thread routine returns; usually what we’ll use
- **Explicit termination**: the thread calls pthread_exit().
- **Process exit**: any thread calls exit(), which terminates the process and all associated threads; maybe not what you really want.
- **Thread cancellation**: another thread calls pthread_cancel() to
terminate a specific thread.
- If thread calls exit() system call, complete process will be terminated which is generally not desirable because the goal is to terminate a thread and not the process, hence we use pthread_exit() as below.
```c
#include <pthread.h>
void pthread_exit(void * retval);
```
- Does not return anything.

### Thread termination functions
```c
void pthread_exit(void *val);
```
- terminates current thread, with a return value equal to the pointer val
- **Note:** Upon termination, a thread releases its runtime stack.
- Therefore the pointer should point to:
    - a global variable, or
    - a dynamically allocated variable.

### Thread Synchronization
```c
#include <pthread.h>
int pthread_join(pthread_t tid, void **retval);
```
- thread calling pthread_join waits for target thread with tid to terminate. Target thread is terminated when it calls pthread_exit()
- Returns 0 on success and error number in case of failure
- **Parameters:**
- **tid** : thread ID of target thread for which calling thread is waiting to terminate
- **retval** : when target thread called pthread_exit(), exit status of the same is returned in retval

### Thread reaping
- Threads are reaped by pthread_join()
```c
int pthread_join(pthread_t tid, void **val)
```
- on success, *val is the return value of the terminated thread.

### Thread Synchronization
- One thread can request another thread belonging to the same process to terminate by calling pthread_cancel()
```c
#include <pthread.h>
int pthread_cancel(pthread_t tid);
```
- Returns 0 on success and error number in case of failure
- **Parameters:**
- **tid** : thread ID of target thread which is requested to be cancelled
- **Note**: Behavior of the target thread will be the same as if it had called pthread_exit()

### Process vs Thread System Calls
Process Primitive | Thread Primitive | Description
--- | --- | --- 
fork | pthread_create | Create a new flow of control
exit | pthread_exit | Exit from an existing flow of control
waitpid | pthread_join | Get exit status from flow of control
atexit | pthread_cancel_push | Register function to be called at exit from flow of control
getpid | pthread_self | Get ID of flow of control
abort | pthread_cancel | Request abnormal termination of flow of control

### Example Program 1
```c
#include "csapp.h"
/* thread routine */
void *mythread(void *vargp) {
    printf("Hello from the other thread\n");
    return NULL;
}
int main() {
    pthread_t tid;
    Pthread_create(&tid, NULL, mythread, NULL);
    printf("Hello from the main thread\n");
    Pthread_join(tid, NULL);
    exit(0);
}
```

### Our previous example
- No assumption about the statement execution order between threads is possible
- The output of the main thread might happen before the output of thread mythread
- printf() is thread-safe
    - Even if called by 2 threads simultaneously, it is guaranteed that outputs won’t be mixed

### Example Program 2
```c
void *get_rand_num(void *args) {
    int *nump = malloc(sizeof(int));
    srand(pthread_self());
    *nump = rand();
    printf("Random number: %d\n", *nump);
    return nump;
}
int main() {
    pthread_t tid;
    void *ptr = NULL;
    pthread_create(&tid, NULL, get_rand_num, NULL);
    pthread_join(tid, &ptr);
    printf("Random number: %d\n", *(int *)ptr);
    return 0;
}
```
##### Notes
- Our previous example had the thread just return from function thread
- In this case the call to pthread_exit() is implicit
- The return value of the function serves as the argument to the (implicitly called) pthread_exit()

### Example Program 3
```c
void *thread (void *vargp) {
    int arg = *((int*)vargp);
    return &arg;
}
int main () {
    pthread_t tid;
    int thread_arg = 0xDEADBEEF;
    int *ret_value;
    pthread_create(&tid, NULL, thread, &thread_arg);
    pthread_join(tid, (void **)(&ret_value));
    printf("%X\n", *ret_value);
    return 0;
}
```

### pthread_exit vs exit
- pthread_exit
    - Terminates the calling thread.
    - Does not terminate the entire process/other threads.
- exit
    - Terminates the entire process
    - All threads are gone
- The main thread is a little special
    - pthread_exit() waits for all threads in the process to terminate

### Different between using exit(), return or pthread_exit()
1) exit() – exit the complete process so if you call from one of the thread
function, it will end the process without waiting for main or any other
thread in the process to continue → process ends

### Example Program 4
```c
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

void *thr_fn(void *arg) {
  printf("child thread id %u is starting \n", (unsigned int)pthread_self());
  printf("child thread id %u is calling exit\n", (unsigned int)pthread_self());
  exit(0);
}

int main(void) {
  pthread_t tid;
  printf("Main thread %u is starting\n", (unsigned int)pthread_self());
  int err = pthread_create(&tid, NULL, thr_fn, NULL);
  sleep(1);
  printf("Main thread %u is finished\n", (unsigned int)pthread_self());
  exit(0);
}
```
```sh
$ ./Program9.out
```
### Output:
```
Main thread 521172736 is starting
child thread id 513017600 is starting
child thread id 513017600 is calling exit
```
**Note:** Message “Main thread 521172736 is finished” is never displayed
because parent also have terminated when child called exit().

2) return() – when called from a thread then it will not wait for any of child
thread to complete → parent and all the child threads ends

```c
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

void *thr_fn(void *arg) {
  printf("child thread id %u is starting \n", (unsigned int)pthread_self());
  sleep(1); // ensure main thread exit first
  printf("child thread id %u is calling exit\n", (unsigned int)pthread_self());
  exit(0);
}
int main(void) {
  pthread_t tid;
  printf("Main thread %u is starting\n", (unsigned int)pthread_self());
  int err = pthread_create(&tid, NULL, thr_fn, NULL);
  sleep(1);
  printf("Main thread %u is finished\n", (unsigned int)pthread_self());
  return 0;
}
```
```sh
$ ./Program10.out
```
```
Main thread 1203812096 is starting
child thread id 1195656960 is starting
Main thread 1203812096 is finished
```
**Note:** Message “child thread id 1195656960 is calling exit” is never displayed because child is terminated by parent calling return

3) pthread_exit() – when called from a thread then it will only end that thread but all other threads in the same process or child threads can continue → this thread ends but all the child threads or any other threads in the same process continues.
```c
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

void *thr_fn(void *arg) {
  printf("child thread id %u is starting \n", (unsigned int)pthread_self());
  sleep(2);
  printf("child thread id %u is calling exit\n", (unsigned int)pthread_self());
  exit(0);
}
int main(void) {
  pthread_t tid;
  printf("Main thread %u is starting\n", (unsigned int)pthread_self());
  int err = pthread_create(&tid, NULL, thr_fn, NULL);
  sleep(1);
  printf("Main thread %u is finished but let the child thread continue\n", (unsigned int)pthread_self());
  pthread_exit(0);
}
```
```sh
$ ./Program11.out
```
```
Main thread 1709131520 is starting
child thread id 1700976384 is starting
Main thread 1709131520 is finished but let the child thread continue
child thread id 1700976384 is calling exit
```
**Note:** pthread_exit() is called by parent/main thread hence it has terminated but child thread continues to execute until it calls exit().

### Complete Thread Example with Synchronization
```c
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

/* Thread Routine */
void *mythread(void *vargp) {
  printf("Thread.a\n");
  printf("Thread.b\n");
  return NULL;
}

int main() {
  pthread_t tid;
  printf("Main.a\n");
  pthread_create(&tid, NULL, mythread, NULL);
  printf("Main.b\n");
  printf("Main.c\n");
  pthread_join(tid, NULL);
  printf("Main.d\n");
  exit(0);
}
```

### Data Race
- A program has a data race if it is possible for a thread to modify an addressable location at the same time that another thread is accessing the same location.
- The result/correctness depends on the sequence/timing of events; i.e., how
things end up being scheduled.

### Example Program 10
```c
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

void *foo(void *vargp)
{
  int id;
  id = *((int *)vargp);
  printf("Thread %d\n", id);
}

int main()
{
  pthread_t tid[3];
  int i;
  for (i = 0; i < 3; i++)
  {
    pthread_create(&tid[i], NULL, foo, &i);
  }
  pthread_join(tid[0], NULL);
  pthread_join(tid[1], NULL);
  pthread_join(tid[2], NULL);
}
```
### Flaw in code
- Inside the loop several threads are created
    - Variable i is visible in all threads
    - Variable i is modified in main after each thread is created
- The threads may not access the parameter fast enough to read the value
“assigned” to them.

### Thread Synchronization
- Mechanism that allows programmer to control relative order of operation occurrence in different threads or processes
- How thread synchronization works:
    - Programmer identifies critical section in the code
    - Implements mutual exclusion to ensure that critical section is mutually exclusive i.e. atomic

### Mutual Exclusion Implementation in POSIX Thread - MUTEX
- MUTEX is like a key to access the critical section that has access to only one thread at a time
```c
#include <pthread.h>
```
- MUTEX variable (containing union of structures) is represented as **pthread_mutex_t** data type defined in /usr/include/bits/pthreadtypes.h Before we can use mutex variable memory allocation has to be done for which use function:
```c
int pthread_mutex_init(pthread_mutex_t *mutex, const pthread_mutexattr_t *attr)
```
which return 0 on success.
- When mutex variable is no longer required memory should be freed for which we
use function:
```c
int pthread_mutex_destroy(pthread_mutex_t *mutex)
```
which return 0 on success.

- To lock mutex: if mutex is already locked by another thread, thread trying to
load mutex will be blocked
```c
int pthread_mutex_lock(pthread_mutex_t *mutex)
```
which return 0 on success
- To lock mutex: if mutex is already locked by another thread, function will
rerurn error code EBUSY
```c
int pthread_mutex_trylock(pthread_mutex_t *mutex)
```
which return 0 on success
- To unlock mutex:
```c
int pthread_mutex_unlock(pthread_mutex_t *mutex)
```
which return 0 on success.

### Avoidance of Race Condition using MUTEX
- Race condition output without using MUTEX (remove all pthread_mutex_* function calls)
```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

pthread_mutex_t mymutex;

void *charatatime(void *str)
{
  char *ptr;
  int c;
  setbuf(stdout, NULL);
  for (ptr = (char *)str; c = *ptr++;)
    putc(c, stdout);
}

int main()
{
  pthread_t thread1, thread2;
  pthread_create(&thread1, NULL, charatatime, "1234567890123456789012345678901234567890");
  pthread_create(&thread2, NULL, charatatime, "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz");
  pthread_join(thread1, NULL);
  pthread_join(thread2, NULL);
  exit(0);
}
```
- With MUTEX implementation never get race condition
```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

pthread_mutex_t mymutex;

void *charatatime(void *str)
{
  char *ptr;
  int c;
  setbuf(stdout, NULL);
  pthread_mutex_lock(&mymutex);
  for (ptr = (char *)str; c = *ptr++;)
    putc(c, stdout);

  pthread_mutex_unlock(&mymutex);
}

int main()
{
  int ret;
  pthread_t thread1, thread2;
  ret = pthread_mutex_init(&mymutex, NULL);
  pthread_create(&thread1, NULL, charatatime, "1234567890123456789012345678901234567890");
  pthread_create(&thread2, NULL, charatatime, "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz");
  pthread_join(thread1, NULL);
  pthread_join(thread2, NULL);
  ret = pthread_mutex_destroy(&mymutex);
  exit(0);
}
```

### Disadvantage of MUTEX
- Deadlock can occur if:
    - Thread locking the same MUTEX twice
    - Thread1 holding lock on mutex1 wants to get lock on mutex2 while thread2 holding lock on mutex2 wants to get lock on mutex1
- Solution: As much as possible use `pthread_mutex_trylock` function
- Only one thread is allowed to lock mutex but in some situations such as Reader-Write problem or Dinning-Philosopher problem multiple locks should be possible.
    - Solution: Reader-Writer Locks and Semaphores

### Example Program 5
```c
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void *foo(void *vargp)
{
  int myid;
  myid = *((int *)vargp);
  free(vargp);
  printf("Thread %d\n", myid);
}

int main()
{
  pthread_t tid[3];
  int i, *ptr;
  for (i = 0; i < 3; i++)
  {
    ptr = malloc(sizeof(int));
    *ptr = i;
    pthread_create(&tid[i], NULL, foo, ptr);
  }
  pthread_join(tid[0], NULL);
  pthread_join(tid[1], NULL);
  pthread_join(tid[2], NULL);
}
```

### Example Program 6
```c
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

void *foo(void *vargp)
{
  int id;
  id = (int)vargp;
  printf("Thread %d\n", id);
}

int main()
{
  pthread_t tid[3];
  int i;

  for (i = 0; i < 3; i++)
    pthread_create(&tid[i], NULL, foo, (void *)i);
  pthread_join(tid[0], NULL);
  pthread_join(tid[1], NULL);
  pthread_join(tid[2], NULL);
}
```

### Previous example
- Pro: No added overhead due to malloc/free
- Con: Assumes that pointer datatype is at least bigger than size of int 
    - May not be true on all systems.

### Threads are not hierarchical
- Unlike processes, a thread doesn’t have a parent or children – threads are peers
- Any thread can pthread_join any other thread
```c
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

void *go1(void *ignored)
{
  sleep(1);
  //pthread_join(*(pthread_t *)ignored, NULL);
  printf("1\n");
  return NULL;
}

void *go2(void *th)
{
  pthread_join(*(pthread_t *)th, NULL);
  printf("2\n");
  return NULL;
}

int main()
{

  pthread_t one, two;

  pthread_create(&one, NULL, go1, NULL);
  pthread_create(&two, NULL, go2, &one);

  //pthread_create(&one, NULL, go1, &two);
  //pthread_create(&two, NULL, go2, NULL);

  pthread_join(two, NULL);
  //pthread_join(one, NULL);

  return 0;
}
```

### Dining-Philosopher Problem
- Five philosophers spend their time eating and thinking.
- They are sitting in front of a round table with spaghetti served.
- There are five plates at the table and five chopsticks set between the plates.
- Eating the spaghetti requires the use of two chopsticks which the philosophers pick up one at a time.
- Philosophers do not talk to each other.
- Semaphore chopstick [5] initialized to 1 Semaphores
- Allow multiple locks
- Semaphore S → integer variable
- Modified by two operations → wait() and signal()
    - wait() – originally called P() for Dutch word “proberen” which means try
    - signal() – originally called V() for Dutch word “verhogen” which means increase
```c
wait(S) {
    while S <= 0; // no-op
    S--;
}
signal(S) {
    S++;
}
```
- Note wait() and signal() are atomic operation

### Thread Synchronization using Semaphores
#### Thread 1
```c
int t; 
wait(sem)
sum = sum + x;
t = sum;
signal(sem);
```

#### Thread2:
```c
int t;
wait(sem)
sum = sum + y;
t = sum;
signal(sem);
```

### Dining-Philosopher Problem: Solution
```c
// The structure of Philosopher i:
do {
    wait ( chopstick[i] ); // lock
    wait ( chopStick[ (i + 1) % 5] ); // lock
    // eat
    signal ( chopstick[i] ); // unlock
    signal (chopstick[ (i + 1) % 5] ); // unlock
    // think
} while (true) ;
```
- Problem is deadlock : when all philosopher decides to eat at the
same time each will pick up one of the 2 chopsticks they need and wait for the other.
- Solution: Allow to lock if both chopsticks are available at the same time
    - Lock the 1 st chopstick
    - For 2nd chopstick, check if it can be locked otherwise release the 1 st one

### POSIX: Semaphores
#### Creating a Semaphore:
```c
int sem_init(sem_t *sem, int pshared, unsigned int value);
```
- initializes a semaphore object pointed to by sem to integer value of “value”
- pshared is a sharing option; a value of 0 means the semaphore is local to the calling process i.e. shared by all threads of the same process; a +ve number indicates it can be shared across multiple processes using shared memory instructions.
#### Terminating a Semaphore:
```c
int sem_destroy(sem_t *sem);
```
- frees the resources allocated to the semaphore sem
- usually called after pthread_join()
- an error will occur if a semaphore is destroyed for which a thread is waiting
#### Semaphore Control:
```c
int sem_post(sem_t *sem); // same as signal()
```
- sem_post atomically increases the value of a semaphore by 1, i.e., when 2 threads call sem_post simultaneously, the semaphore's value will also be increased by 2 (there are 2 atoms calling).
```c
int sem_wait(sem_t *sem); // same as wait()
```
- sem_wait atomically decreases the value of a semaphore by 1; but always waits until the semaphore has a non-zero value first.

### Example Program 16 
```c
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

sem_t semaphore;

void threadfunc() {
  while (1) {
    sem_wait(&semaphore);
    printf("Hello from thread!\n");
    sem_post(&semaphore);
    sleep(1);
  }
}

int main(void) {
  /* initialize semaphore, only to be used with threads in this process, set value to 1 */
  sem_init(&semaphore, 0, 1);
  pthread_t *mythread;
  mythread = (pthread_t *)malloc(sizeof(*mythread));
  
  // start the thread
  printf("Starting thread, semaphore is unlocked.\n");
  pthread_create(mythread, NULL, (void *)threadfunc, NULL);

  getchar(); // While "Hello from thread!" is displayed, press Enter

  sem_wait(&semaphore);
  printf("Semaphore locked.\n");
  
  // do stuff with whatever is shared between threads
  getchar();
  printf("Semaphore unlocked.\n");
  sem_post(&semaphore);
  getchar();
  return 0;
}
```

### Example Program 17 : Dining Philosopher's Problem
```c
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define N 5
#define THINKING 2
#define HUNGRY 1
#define EATING 0
#define LEFT (phnum + 4) % N
#define RIGHT (phnum + 1) % N

int state[N];
int phil[N] = {0, 1, 2, 3, 4};

sem_t mutex;
sem_t S[N];

void test(int phnum) {
  if(state[phnum] == HUNGRY && state[LEFT] != EATING && state[RIGHT] != EATING) {
    // State that Eating
    state[phnum] = EATING;
    sleep(2);
    printf("Philosopher %d takes fork %d and %d\n",
           phnum + 1, LEFT + 1, phnum + 1);
    printf("Philosopher %d is Eating\n", phnum + 1);
    // sem_post(&S[phnum]) has no effect
    // during takefork
    // used to wake up hungry philosophers
    // during putfork
    sem_post(&S[phnum]);
  }
}

// take up chopsticks
void take_fork(int phnum) {
  sem_wait(&mutex);
  // state that hungry
  state[phnum] = HUNGRY;
  printf("Philosopher %d is Hungry\n", phnum + 1);
  // eat if neighbours are not eating
  test(phnum);
  sem_post(&mutex);
  // if unable to eat wait to be signalled
  sem_wait(&S[phnum]);
  sleep(1);
}

// Put Down Chopsticks
void put_fork(int phnum) {
  sem_wait(&mutex);
  // State that thinking
  state[phnum] = THINKING;
  printf("Philosopher %d putting fork %d and %d down\n", phnum + 1, LEFT + 1, phnum + 1);
  printf("Philosopher %d is thinking\n", phnum + 1);
  test(LEFT);
  test(RIGHT);
  sem_post(&mutex);
}

void *philospher(void *num) {
  while (1) {
    int *i = num;
    sleep(1);
    take_fork(*i);
    sleep(0);
    put_fork(*i);
  }
}

int main() {
  int i;
  pthread_t thread_id[N];
  
  // Initialize the Semaphores
  sem_init(&mutex, 0, 1);
  for (i = 0; i < N; i++) {
    sem_init(&S[i], 0, 0);
  }
  // Create Philosopher Processes
  for (i = 0; i < N; i++) {
    pthread_create(&thread_id[i], NULL, philospher, &phil[i]);
    printf("Philosopher %d is thinking\n", i + 1);
  }
  for (i = 0; i < N; i++) {
    pthread_join(thread_id[i], NULL);
  }
}
```



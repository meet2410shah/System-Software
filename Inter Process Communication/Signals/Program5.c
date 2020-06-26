#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void signal_handler() {
	printf("process %d, %d got a SIGINT\n", getpid(), getpgid(getpid()));
}

int main(void) {
	signal(SIGINT, signal_handler);
	if (fork() == 0) {                      // Child Process
        setpgid(getpid(), 0);	            // Set Process group as pid for child
        printf("child pid %d and child group %d waits\n", getpid(), getpgid(0));
    } else {	                            // Parent Process
		printf("parent pid %d and parent group %d waits\n", getpid(), getpgid(0));
    }
	pause();	                            // Wait for Signal
}
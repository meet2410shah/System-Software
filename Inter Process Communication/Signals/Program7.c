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
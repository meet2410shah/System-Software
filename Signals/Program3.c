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
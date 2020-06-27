#include <stdio.h>
#include <unistd.h>

int main(void) {
	alarm(3);
	printf("Looping forever\n");
	while(1);
	printf("This point is never reached");
}
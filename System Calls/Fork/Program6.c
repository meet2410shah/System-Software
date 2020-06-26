#include <stdio.h>
#include <unistd.h>

void main() {
    printf("LO\n");
    if(fork() != 0) {
        printf("L1\n");
        if(fork() != 0) {
            printf("L2\n");
            fork();
        }
    }
    printf("Bye\n");  
}
#include <stdio.h>
#include <unistd.h>

void main() {
    printf("LO\n");
    fork();
    printf("L1\n");
    fork();
    printf("Bye\n");
}
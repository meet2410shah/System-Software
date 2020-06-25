#include <stdio.h>
#include <unistd.h>

void main() {
    if(fork() == 0) {
        printf("I am a Child Process\n");
    } else {
        printf("I am a Parent Process\n");
    }
}
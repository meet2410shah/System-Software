#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
   int i;
   for (i = 1; i < argc; i++) {
      execvp(argv[i], &(argv[i]));
    }
   return 0;
}

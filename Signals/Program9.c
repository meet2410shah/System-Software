#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define	READ	0               /* The index of the "read" end of the pipe */ 
#define	WRITE	1               /* The index of the "write" end of the pipe */ 

char*  phrase = "Stuff this in your pipe and smoke it";

int main(void) {
	int fd[2], bytesRead; 
	char message[100];          /* Parent process' message buffer */
	pipe(fd);                   /* Create  an unnamed pipe */
	if(fork() == 0) {           /* Child, write - reader role possible */
		close(fd[READ]);        /* Close unused end */
		write(fd[WRITE], phrase, strlen(phrase)+1); /* Send */
		close(fd[WRITE]);       /* Close used end */
	} else {                    /* Parent, reader */
		close(fd[WRITE]);       /* Close unused end - writer role possible */
		bytesRead = read( fd[READ], message, 100 ); /* Receive */
		printf("Read %d bytes: %s \n", bytesRead, message );
		close(fd[READ]);        /* Close used end */ 
	}
}
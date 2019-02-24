/**********************************************
 * Name: Travis Laxson
 * Date: 19 Feb 2019
 * Description: Source file for small custom
 * shell built in C99 with built-in commands, 
 * a shell prompt with responsive command line
 * execution including signal handline.
 **********************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>

/* Global constants */
#define MAX_CHARS 2048
#define MAX_ARGS 512

int main()
{
	bool exitShell = false;
	size_t bufferSize = MAX_CHARS;
	size_t chars;
	char* input = NULL;
	char* token;

	do{
		printf(": ");
		fflush(stdout);
		chars = getline(&input, &bufferSize, stdin);
	/*	DEBUG	printf("Number of chars: %zu\n", chars);
	*/	/* parse input */
		token = strtok(input, "\n ");
	/*	DEBUG	printf("Chars: %s\n", token);
		fflush(stdout);
		*/
		if(strcmp(input, "status") == 0){
			/* Print the exit status 0 if no foreground command or terminating signal of last foreground process */
			printf("Entered 'status' - Input = %s\n", token);
			/* DEBUG - Print finished */
			exitShell = false;
			printf("Exit status = %d\n", exitShell);
		}
		else if(strcmp(input, "exit") == 0){
			/* Kill all background processes */
			printf("Killing all processes\n");
			exitShell = true;	
		}	
		else if(strcmp(input, "cd") == 0){
			/* Check arguments and then change directory based on provided arg */
			printf("Entered 'cd' - Input = %s\n", token);
		}
		else{
			/* Execute new process with command(s) */
			printf("Fork new process with command\n");
		}
	}while(!exitShell);

	return 0;
}

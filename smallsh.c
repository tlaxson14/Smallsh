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

/* Function declarations */
char* readInput(void)
{
	size_t chars;
	size_t bufferSize = MAX_CHARS;
	char* input = NULL;
	
	/* Read user input line from std input */
	chars = getline(&input, &bufferSize, stdin);
	/* DEBUG: Print number of chars	 */
 	printf("Num chars: %d\n", chars);
	/* DEBUG: Print line  */
	printf("User input: %s\n", input);
	/* Flush std output buffer */	
	fflush(stdout);
	return input;
}



int main()
{
	bool exitShell = false;
	char* token;
	char* inputLine;


	do{
		printf(": ");
		fflush(stdout);

		/* read user input */
		inputLine = readInput();

		/* Parse user input into list of commands */
/*
		token = strtok(inputLine, "\n ");
*/
		if(strcmp(inputLine, "status") == 0){
			/* Print the exit status 0 if no foreground command or terminating signal of last foreground process */
			printf("Entered 'status' - Input = %s\n", inputLine);
			/* DEBUG - Print finished */
			exitShell = false;
			printf("Exit status = %d\n", exitShell);
		}
		else if(strcmp(inputLine, "exit") == 0){
			/* Kill all background processes */
			printf("Killing all processes\n");
			exitShell = true;	
		}	
		else if(strcmp(inputLine, "cd") == 0){
			/* Check arguments and then change directory based on provided arg */
			printf("Entered 'cd' - Input = %s\n", inputLine);
		}
		else{
			/* Execute new process with command(s) */
			printf("Fork new process with command\n");
		}
	}while(!exitShell);

	return 0;
}

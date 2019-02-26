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

/***********************
*   GLOBAL CONSTANTS   *
************************/
#define MAX_CHARS 2048
#define MAX_ARGS 512

/************************
* FUNCTION DECLARATIONS *
*************************/
char* readUserInput(void);
char** parseUserInput(char*);


int main(void)
{
	bool exitShell = false;
	char** argsList;
	char* inputLine;
	int count = 0;

	/* Run shell loop logic */
	do{
		printf(": ");
		fflush(stdout);

		/* Read user input */
		inputLine = readUserInput();

		/* Parse user input into list of arguments */
		argsList = parseUserInput(inputLine);
		
		/* Debug: Print returned args list */
	/*	while(argsList != NULL) {
			printf("ArgsList = %s\n", argsList[count]);
			count++;
		}
	*/	
		/* Check parsed commands for built in functions */
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

/***********************
* FUNCTION DEFINITIONS *
************************/

/*******************************************************
		READ USER INPUT FUNCTION
********************************************************
* Name: readUserInput
* Description: 
  	Initializes charater input pointer
  	and reads user input from stdin, debugs,
  	and prints string input to console before 
  	returning the string.
* Input:
  	 N/A
* Ouput: 
  	Debugging info - number of chars and input
* Returns:
  	User string input read from getline function
* Sources:
  	Linux man page: http://man7.org/linux/man-pages/man3/getline.3.html
 	CforDummies: https://c-for-dummies.com/blog/?p=1112
 	Tutorial: https://brennan.io/2015/01/16/write-a-shell-in-c/
******************************************************/
char* readUserInput(void)
{
	char* input = NULL;
	size_t chars;
	size_t bufferSize = MAX_CHARS;
	
	/* Read user input line from std input */
	chars = getline(&input, &bufferSize, stdin);
	/* DEBUG: Print number of chars	 */
 	printf("Num chars: %zu\n", chars);
	/* DEBUG: Print line  */
	printf("User input: %s\n", input);
	/* Flush std output buffer */	
	fflush(stdout);
	return input;
}

/******************************************************
     PARSE USER INPUT INTO SHELL COMMANDS FUNCTION     
*******************************************************
* Name: parseUserInput
* Description: 
* Input:
  	Pointer to character/string input from returned 
	readUserInput function 
* Ouput: 
  	Debugging info - number of chars and input
* Returns:
  	Pointer to array of character pointers containing
	parsed list of command line args input by the user 
* Sources:
	CforDummies: https://c-for-dummies.com/blog/?p=1758
	Man pages: http://man7.org/linux/man-pages/man3/strtok.3.html
	G4Gs: https://www.geeksforgeeks.org/how-to-split-a-string-in-cc-python-and-java/
	Tutorial: https://brennan.io/2015/01/16/write-a-shell-in-c/
******************************************************/
char** parseUserInput(char* inputLine)
{
	char* arg;
	char** argsList = (char**)malloc(MAX_ARGS * sizeof(char*));
	int indx = 0;

	/* DEBUG - check memory allocation successful */
	if(!argsList) {
		fprintf(stderr, "Memory allocation error.\n");
		exit(1);
	}
	/*size_t argsMax = MAX_ARGS;*/		/* Max number of allowed args */
	/* Tokenize the string of characters from user input for first argument */
	/* Argument delimiters = space, newline, tab (in order) */
	arg = strtok(inputLine, " \n\t");
	/* Process each token until null terminator found */
	while(arg != NULL){
		/* Store argument in argument array at i-th index */
		argsList[indx] = arg;
		/* Increment index in args array */
		indx++;
		/* Update string argument tokenizer for next argument */
		arg = strtok(NULL, " \n\t");
	}
	/* Assign last index to NULL */
	argsList[indx] = NULL;
	/* DEBUG: Print argument */
	indx = 0;
	while(argsList[indx] != NULL){
		printf("Argument %d: %s\n", indx+1, argsList[indx]);
		indx++;
	}
	return argsList;
}

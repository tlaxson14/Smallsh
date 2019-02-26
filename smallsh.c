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
	bool exitShell = false;		/* Shell exit status flag var */
	char* inputLine;		/* Stores user input from shell */
	char** argsList;		/* Stores tokenized arguments from user input */
	int count = 0;			/* DEBUG counter var */ 

	/* Run shell loop logic */
	do{
		printf(": ");
		fflush(stdout);

		/* Read user input */
		inputLine = readUserInput();

		/* Parse user input into list of arguments */
		argsList = parseUserInput(inputLine);	

		/* DEBUG: Print returned char strings from args list */	
		/* for(count = 0; count < INSERT NO. OF ARGS /); count++){
			printf("Size of argsList = %d\n", sizeof(argsList));
			printf("Argument %d: %s\n", count+1, argsList[count]);
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

	/* Deallocate heap memory used for user input and arguments array */
	free(inputLine);
	free(argsList);
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
	/* Allocate memory up to max characters allowed */
	char* inputBuffer = malloc(MAX_CHARS * sizeof(char));
	size_t chars;
	size_t bufferSize = MAX_CHARS;
	
	/* Read user input line from std input */
	chars = getline(&inputBuffer, &bufferSize, stdin);
	/* DEBUG: Print number of chars	 */
 	printf("Num chars: %zu\n", chars);
	/* DEBUG: Print line  */
	printf("User input: %s\n", inputBuffer);
	/* Flush std output buffer */	
	fflush(stdout);
	
	return inputBuffer;
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
	char** argsArr = (char**)malloc(MAX_ARGS * sizeof(char*));
	int indx = 0;

	/* DEBUG - check memory allocation successful */
	if(!argsArr) {
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
		argsArr[indx] = arg;
		/* Increment index in args array */
		indx++;
		/* Update string argument tokenizer for next argument */
		arg = strtok(NULL, " \n\t");
	}
	/* Assign last index to NULL */
	argsArr[indx] = NULL;

	/* DEBUG: Print all indexed arguments */
	indx = 0;
	while(argsArr[indx] != NULL){
		printf("Argument %d: %s\n", indx+1, argsArr[indx]);
		indx++;
	}

	return argsArr;
}
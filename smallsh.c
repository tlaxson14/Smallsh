/**********************************************
 * Name: Travis Laxson
 * Date: 19 Feb 2019
 * Description: Source file for small custom
 * shell built in C99 with built-in commands, 
 * a shell prompt with responsive command line
 * execution including signal handline.
 **********************************************/

/* Source: Removes warning with getline function call */
/* https://stackoverflow.com/questions/8480929/scratchbox2-returns-implicit-declaration-of-function-getline-among-other-weir */
#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>

/***********************
*   GLOBAL CONSTANTS   *
************************/
#define MAX_CHARS 2048
#define MAX_ARGS 512

int argsCount;		/* Stores number of command arguments from user input */
bool redirectInput;	/* True if "<" input redirection entered, else false */
bool redirectOutput;	/* True if ">" output redirection entered, else false */
char* inputFileName;	/* Holds the file name of the input file when input redirection used */
char* outputFileName;	/* Holds the file name of the output file when out redirection used */

/************************
* FUNCTION DECLARATIONS *
*************************/
char* readUserInput(void);
char** parseUserInput(char*);
bool executeUserInput(char**);

int main()
{
	bool shellStatus;		/* Shell exit status flag var */
					/* int count = 0  DEBUG counter var */ 
	/* Run shell loop logic */
	do{

		char* inputLine;	/* Stores user input from shell */
		char** argsList;	/* Stores tokenized arguments from user input */
		
		/* Reset number of arguments */
		argsCount = 0;		

		/* Print prompt */
		printf(": ");

		/* Flush stdout */
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
		/* Execute shell commands */
		shellStatus = executeUserInput(argsList);

		/* Deallocate heap memory used for user input and arguments array */
		free(inputLine);
		free(argsList);

	}while(!shellStatus);

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
	CS344 Lecture Notes: 3.3 Advanced User Input with getline() about stuck errors
******************************************************/
char* readUserInput(void)
{
	char* inputBuffer = NULL;/* = malloc(MAX_CHARS * sizeof(char));*/
	size_t charsEntered;						
	size_t bufferSize = 0;

	/* Read user input line from std input */
	charsEntered = getline(&inputBuffer, &bufferSize, stdin);
	/* DEBUG: Print number of chars	 */
 	if(charsEntered == -1){
		/* printf("Input error: \n"); */
		/* Remove error status from stdin before resuming upon interruption */
		clearerr(stdin);
	}

	/* Flush std output buffer */	
	fflush(stdout);
	/* DEBUG: Display allocated bytes in memory, character count, and user input */
/*	printf("Allocated bytes: %zu\nCharacter count: %zu\nUser input: %s\n", bufferSize, charsEntered, inputBuffer); 
*/	
	/* Flush std output buffer */	
/*	fflush(stdout);
*/
	return inputBuffer;
}

/******************************************************
     PARSE USER INPUT INTO SHELL COMMANDS FUNCTION     
*******************************************************
* Name: parseUserInput
* Description: 
	Parses the user input using a tokenizer and 
	inserts the arguments into an array of pointers 
	to string characters before returning this 
	array of character pointers. Modifies the global
	variables for number of arguments, redirection
	operators "<", ">", and assigns the input and
	output file names if redirection operators
	invoked by the user.
* Input:
  	(1) - Character pointer
		Pointer to character/string input from 
		returned readUserInput function 
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
	Redirection: https://stackoverflow.com/questions/11515399/implementing-shell-in-c-and-need-help-handling-input-output-redirection
******************************************************/
char** parseUserInput(char* inputLine)
{
	char* arg = NULL;
	char** argsArr = (char**)malloc(MAX_ARGS * sizeof(char*));
	int indx = 0;
	
	/* Global singal handlers */
	argsCount = 0;
	redirectInput = false;
	redirectOutput = false;

	/* DEBUG - check memory allocation successful */
	if(!argsArr) {
		printf("Memory allocation error.\n");
		exit(1);
	}
	/*size_t argsMax = MAX_ARGS;*/		/* Max number of allowed args */
	/* Tokenize the string of characters from user input for first argument */
	/* Argument delimiters = space, newline, tab (in order) */
	arg = strtok(inputLine, " \n\t");
	
	/* Error check for arg */
	if(arg == NULL){
		argsArr[0] = NULL;
		return argsArr;
	}

	/* Process each token until null terminator found */
	while(arg != NULL){
		/* Check if redirection operators entered */
		if(strcmp(arg, "<") == 0){
			redirectInput = true;
			/* DEBUG: Print confirmation 
  			printf("Entered input redirection operator\n");
			*/
		}
		else if(strcmp(arg, ">") == 0){
			redirectOutput = true;
			/* DEBUG: Print confirmation 
			printf("Entered output redirection operator\n");
			*/
		}
		else{
			/* If input redirection assign tokenized arg as input file */
			if(redirectInput){
				inputFileName = arg;
			}
			/* If output redirection assign tokenized arg as output file */
			else if(redirectOutput){
				outputFileName = arg;
			}		
			/* Store argument in argument array at i-th index */
			else{
				argsArr[indx] = arg;
				
				/* Increment index in args array */
				indx++;
				
				/* Increment global args count */
				argsCount++;
			}
		}
		/* Update string argument tokenizer for next argument */
		arg = strtok(NULL, " \n\t");
	}
	/* Assign last index to NULL */
	argsArr[indx] = NULL;

	/* DEBUG: Print all indexed arguments */
	/*indx = 0;
	while(argsArr[indx] != NULL){
		printf("Argument %d: %s\n", indx+1, argsArr[indx]);
		indx++;
	}
	*/
	return argsArr;
}

/*******************************************************
    EXECUTE COMMANDS PROVIDED BY USER INPUT FUNCTION  
********************************************************
* Name: executeUserInput
* Description: 
	Executes the shell commands (if possible) 
	provided by the user and returns the boolean 
	flag status of the shell.
* Input:
	(1) - Pointer to array of char pointers
* Ouput: 
  	Debugging info - number of chars and input
* Returns:
	Boolean flag variable status (if true, terminates)
* Sources:
	Tutorial: https://brennan.io/2015/01/16/write-a-shell-in-c/
	strncmp: https://linux.die.net/man/3/strncmp
******************************************************/
bool executeUserInput(char** argsArr)
{
	bool exitShell;
	int i = 0;	

	/* If no user input, return false to keep iterating shell loop */	
	if(argsArr[0] == NULL){
		exitShell = false;
		return exitShell;
	}
	/* Check parsed commands for built in functions */
	/* Check if first byte is the comment indicator '#' */
	if(strncmp(argsArr[0], "#", 1) == 0){
		/* DEBUG: Print comment */
		printf("DEBUG: Printed comment\n");
		while(argsArr[i] != NULL){
			printf("%s ", argsArr[i]);
			i++;
		}
		printf("\n");	
		exitShell = false;
	}
	else if(strcmp(argsArr[0], "status") == 0){
	/* Print the exit status 0 if no foreground command or terminating signal of last foreground process */
		/*printf("Entered 'status' - Input = %s\n", *argsArr);*/
		/* DEBUG - Print finished */
		exitShell = false;
		printf("Exit status = %d\n", exitShell);	
		/*	return exitShell; */
	}
	else if(strcmp(argsArr[0], "cd") == 0){
		/* Check arguments and then change directory based on provided arg */
		/* printf("Entered 'cd' - Input = %s\n", *argsArr);*/
		/* If user only enters 'cd' without any additional arguments then navigate to home directory */
		if(argsArr[1] == NULL){
			char* path = getenv("HOME");
			chdir(path);
			/* Error check */
			if(chdir(path) != 0){
				/* Print error number and description */
				printf("ERROR %d: %s\n", errno, strerror(errno));
			}
			/* DEBUG: Print path */
			printf("Path = %s\n", path);
			exitShell = false;
		}
		/* User enters additional argument to change into desired directory */
		else if(argsArr[1] != NULL){
			char* newDir = argsArr[1];
			/* DEBUG: Print user input for new directory to navigate to */
			printf("Entered new dir = %s\n", newDir);
			/* Change into directory */
			chdir(newDir);
			/* Error check */
			if(chdir(newDir) != 0){
				/* Print error number and description */
				printf("ERROR %d: %s\n", errno, strerror(errno));
			}
			exitShell = false;
		}
		/*return exitShell;*/
	}
	else if(strcmp(argsArr[0], "exit") == 0){
		/* Kill all background processes */
		/*printf("Killing all processes\n");*/
		exitShell = true;
	/*	return exitShell;*/
	}
	/* Check for input/output redirection */
	else if(redirectInput){
		printf("- Redirected input -\nEntered %d arguments\n", argsCount);
	}
	else if(redirectOutput){
		printf("- Redirected output -\nEntered %d arguments\n", argsCount);
	}
	else{
		/* Code source: Lecture 3.1 - Processes */
		pid_t spawnPid = -5;
		int childExitMethod = -5;

		spawnPid = fork();
		if(spawnPid == -1){
			perror("ERROR!\n");
			exit(1);
		}
		else if(spawnPid == 0){
			printf("CHILD PROCESS: PID = %d, exiting!\n", spawnPid);
			/* Place command execution logic in child here */
			exit(0);
		}
		printf("PARENT PROCESS: PID = %d, waiting...\n", spawnPid);
		waitpid(spawnPid, &childExitMethod, 0);
		printf("PARENT PROCESS: Child process terminated, exiting!\n");

		if(WIFEXITED(childExitMethod)){
			printf("The process exited normally!\n");
			int exitStatus = WEXITSTATUS(childExitMethod);
			printf("Exit status: %d\n", exitStatus);
		}
		else if(WIFSIGNALED(childExitMethod)){
			printf("The process exited normally!\n");
			int termSignal = WTERMSIG(childExitMethod);
			printf("Signal termination: %d\n", termSignal);
		}

		exit(0);	

		/* Execute new process with command(s) */
		/*printf("Fork new process with command\n");*/
		/*exitShell = false;*/
		/*return exitShell;*/
	}
	return exitShell;
}

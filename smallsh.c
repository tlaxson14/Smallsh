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
	bool finished = false;
	char input[MAX_CHARS];
	int count = 0;
	size_t lineBuffer = MAX_CHARS;
	size_t chars = 0;

	do{
		sleep(1);
		printf(": ");
		fflush(stdout);
		chars = getline(&input, &lineBuffer, stdin);
		printf("Number of chars: %zu\n", chars);
		fflush(stdout);
		count++;
		if(count == 3){
			printf("Closing\n");	
			fflush(stdout);
			finished = true;	
		}
	}while(!finished);

	return 0;
}

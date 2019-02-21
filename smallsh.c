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

	do{
		printf("In do while: %d\n", count);
		sleep(1);
		printf(": ");
		gets(input);
		printf("Printing input: %s\n", input);
		count++;
		if(count == 3){
			printf("Closing\n");	
			finished = true;	
		}
	}while(!finished);

	return 0;
}

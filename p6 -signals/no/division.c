////////////////////////////////////////////////////////////////////////////////
// Main File:        mySigHandler.c
// This File:        division.c
// Other Files:      mySigHandler.c sendsig.c 
// Semester:         CS 354 Spring 2021
// Instructor:       deppeler
//
// Author:           Nicole Welsh
// Email:            newelsh@wisc.edu
// CS Login:         nwelsh
//
/////////////////////////// OTHER SOURCES OF HELP //////////////////////////////
//                   fully acknowledge and credit all sources of help,
//                   other than Instructors and TAs.
//
// Persons:          Identify persons by name, relationship to you, and email.
//                   Describe in detail the the ideas and help they provided.
//
// Online sources:   avoid web searches to solve your problems, but if you do
//                   search, be sure to include Web URLs and description of
//                   of any information you find.
//////////////////////////// 80 columns wide ///////////////////////////////////

 #include <stdio.h>
 #include <signal.h>
 #include <unistd.h>
 #include <string.h>
 #include <stdlib.h>

 //global variable for counter
 int counter = 0;
 
 /* 
  * SIGFPE_handler is a function that gets called when the user tries to divide by 0. 
  * This says the number of successful operations and exits.
  */
 void SIGFPE_handler() {
	//ends the program when there is a divide by 0 error. says total number of ops
 	printf("Error: a division by 0 operation was attempted.\n");
	printf("Total number of operations completed successfully: %d\n", counter);
	printf("The program will be terminated.\n");
	exit(0);
 }
 /* 
  * SIGINT_handler is a function that gets called when the user types Ctrl-C. This tells
  * the user the total number of operations and terminates the program. 
  */
 void SIGINT_handler() {
	//ends the program and says the numper of operations
	printf("\nTotal number of operations successfully completed: %i\n", counter);
	printf("The program will be terminated.\n");
	exit(0);
 }

 /*  
  * Main function divides two inputted numbers by the user and calculates the remainder. Exits main when 
  * user tries to divide by 0 or when user types Ctrl-C. 
  */
 int main(){
	//structures for action 1 and 2 
	struct sigaction act, act2;
	//input 1 and 2 from user 
	char input1[100];
	char input2[100];
	//int variables: int 1 and 2, divide 1/2, mod 1 and 2
	int int1, int2, divide, remainder;

	//this is like mySigHandler, calling the same way 
	memset(&act, 0, sizeof(act));
	act.sa_handler = &SIGFPE_handler;
	//check sigaction: if there is an error calling SIGFPE
	if (sigaction(SIGFPE, &act, NULL) != 0) {
		printf("Error with SIGFPE\n");
		exit(1);
	}

	//again, like above part, calls SIGINT_handler
	memset (&act2, 0, sizeof(act2));
	act2.sa_handler = SIGINT_handler;	
	//check sigaction: if there is an error with SIGINT
	if (sigaction (SIGINT, &act2, NULL) != 0) {
		printf("Error with SIGINT\n");
		exit(1);
	}

	//infinite loop until user tries to divide by 0 or user calls Ctrl-C
	while(1){
		//get input from user for first integer
		printf("Enter first integer: ");
		//check fgets: if there is no input, input invalid and exit
		if(fgets(input1, sizeof(input1), stdin) == NULL) { 
         		printf("input invalid\n");
            		exit(1);
        	}
		//uses atoi to get the user number
		int1 = atoi(input1);

		//same as first int, for second int
		printf("Enter second integer: ");
		//check fgets: if error with number, return and exit
                if(fgets(input2, sizeof(input2), stdin) == NULL) {
                        printf("input invalid\n");
                        exit(1);
                }
		//get integer 2 with atoi
                int2 = atoi(input2);

		//calculate divide and remainder 
		divide = int1 / int2;
		remainder = int1 % int2;

		//print the ints, division, and remainder. increment counter
		printf("%i / %i is %i with a remainder of %i\n", int1, int2, divide, remainder);
		counter++;
	}

	return(0);
}
//end of divide.c

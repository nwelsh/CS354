////////////////////////////////////////////////////////////////////////////////
// Main File:        mySigHandler.c
// This File:        mySigHandler.c
// Other Files:      sendsig.c division.c
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

 #include <signal.h>
 #include <string.h>
 #include <stdio.h>
 #include <stdlib.h>
 #include <time.h>
 #include <unistd.h>
 #include <sys/types.h>
 
 //global varibles for alarm time and counter for SIGUSR
 int alarmTime = 3;
 int counter = 0;
 /*
  * SIGALRM_handler called from main that prints the PID and current time every 3 seconds. Then re-arms the alarm
  * and returns to main. 
  *
  */
 void SIGALRM_handler() {
	time_t currentTime; 

	//print out the PID with getpid
	printf("PID: %d ", getpid());

	//check time return value: if time gets set incorrectly, produce error
	if(time(&currentTime) == -1)
	{
		printf("Error in time, time is negative\n");
		exit(1);
	}

	//print out the current time with ctime in correct format
	printf("CURRENT TIME: %s", ctime(&currentTime));

	//check ctime return value: if time is null, return error
	if(ctime(&currentTime) == NULL)
	{
		printf("Error in time, time is null\n");
		exit(1);
	}

	//re-arm the alarm
	alarm(alarmTime);
 }

 /*
  * SIGUSR_handler is called from act2 and tells user that recieved a SIGUSR signal,
  * then increments counter and returns to main. 
  *
  */
 void SIGUSR1_handler(){
	//print message to user and add to counter for part 3 
	printf("SIGUSR1 handled and counted!\n");
	counter++;
 }
 /*
  * SIGINT_handler is a function that exits the running while loop when the user 
  * signals by saying kill SIGINT. This function prints out the amount of times 
  * SIGUSR1 was handled and exits the program like Ctrl-C.
  */
 void SIGINT_handler(){
	//print message saying recieved and amount of times SIGUSR1 called, then call exit
	printf("SIGINT handled.\n");
	printf("SIGUSR1 was handled %i times. Exiting now.\n", counter);
	exit(0);
 }
 /*
 * Main method that calls struct act, act2, and act3. These are structures that a user and 
 * view the PID and current time and can kill with SIGUSR or SIGINT, which will count the 
 * amount times the user calls and then if SIGINT is called, ends. 
 */
 int main() {

	//structures for the three parts: action 1 2 and 3
	struct sigaction act, act2, act3; 

	//part 1: Setting up the Alarm
	//set up the instructions 
	printf("PID and time print every 3 seconds.\n");
	printf("Type Ctrl-C to end the program.\n");

	//set up an alarm that will go off every 3 seconds, causing SIGALARM signal to be sent 
	alarm(alarmTime);
	
	//register a signal handler to handle SIGALARM, call another function: handler_SIGALM
	memset(&act, 0, sizeof(act));          
	act.sa_handler = &SIGALRM_handler;	 //sa_handler assign to handler_SIGALM function
	//check sigaction return value: if sigaction is not cleared, error
	if(sigaction(SIGALRM, &act, NULL) != 0) 
	{
		printf("Error with signal handler");
		exit(1);
	}
	 
	//part 2: User Defined Signals  
	//print a message on recieving SIGUSR1 signal 
	memset(&act2, 0, sizeof(act2)); //this section mainly the same as part 1
	act2.sa_handler = &SIGUSR1_handler; //user defined signal
	//check sigaction: if user types SIGUSR1
	if(sigaction (SIGUSR1, &act2, NULL) != 0)
	{
		printf("Error with SIGUSR");
		exit(1);
	}

	//part 3: Handling Ctrl-C, the Keyboard Interrupt Signal
	//change what happens when Ctrl-C is typed 
	memset(&act3, 0, sizeof(act3));
	act3.sa_handler = &SIGINT_handler;
	//check sigaction
	if(sigaction (SIGINT, &act3, NULL) != 0)
	{
		printf("Error with SIGINT");
		exit(1);
	}
	
	//infinite loop to be run
	while(1)
	{
  	}
  	return 0;
}
//end my mySigHandler.c
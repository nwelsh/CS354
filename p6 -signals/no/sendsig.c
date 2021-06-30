////////////////////////////////////////////////////////////////////////////////
// Main File:        mySigHandler.c
// This File:        sendsig.c
// Other Files:      mySigHandler.c division.c
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

#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <stdio.h>

 /*
  * Main method calls from user in another terminal to the running terminal. If the user 
  * inputs the incorrect arguments, returns usage error. If the user inputs -u, calls 
  * SIGUSR. If the user inputs -i, calls SIGINT.
  */
 int main(int argc, char *argv[]) {

	//if arguments are not sendsig, signal type, and PID number, exit
	if(argc != 3){
		printf("Usage: sendsig <signal type> <pid>\n");
		exit(0);
	}

	//else... if input i or u, do task 
	//if first argument is -u for SIGUSR
	if(strcmp(argv[1], "-u") == 0)
        {
                //kill the second argument as SIGUSR1. if fails, return error
		if(kill(atoi(argv[2]), SIGUSR1) == -1)
                {
                        printf("Error with kill");
                        exit(1);
                }

        }
        //if first argument is -i for SIGINT - end 
        else if(strcmp(argv[1], "-i") == 0)
        {
		//kill with the second argument as SIGINT. if fails, return error
                if(kill(atoi(argv[2]), SIGINT) == -1)
                {
                        printf("Error with kill");
                        exit(1);
                }

	}
	return 0;
}
//end of sendsig.c

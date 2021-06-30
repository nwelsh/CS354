///////////////////////////////////////////////////////////////////////////////
//
// Copyright 2021 Deb Deppeler
// Posting or sharing this file is prohibited, including any changes/additions.
//
// We have provided comments and structure for this program to help you get 
// started.  Later programs will not provide the same level of commenting,
// rather you will be expected to add same level of comments to your work.
//
////////////////////////////////////////////////////////////////////////////////
// Main File:        checkboard.c
// This File:        checkboard.c
// Other Files:      board1.txt board1solutions.txt
// Semester:         CS 354 Spring 2021
//
// Author:           Nicole Welsh 
// Email:            newelsh
// CS Login:         nwelsh
//
/////////////////////////// OTHER SOURCES OF HELP //////////////////////////////
//                   Fully acknowledge and credit all sources of help,
//                   other than Instructors and TAs.
//
// Persons:          Identify persons by name, relationship to you, and email.
//                   Describe in detail the the ideas and help they provided.
//
// Online sources:   Avoid web searches to solve your problems, but if you do
//                   search, be sure to include Web URLs and description of
//                   of any information you find.
////////////////////////////////////////////////////////////////////////////////
   
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
     
char *DELIM = ",";  // commas ',' are a common delimiter character for data strings
     
/* COMPLETED (DO NOT EDIT):       
 * Read the first line of input file to get the size of that board.
 * 
 * PRE-CONDITION #1: file exists
 * PRE-CONDITION #2: first line of file contains valid non-zero integer value
 *
 * fptr: file pointer for the board's input file
 * size: a pointer to an int to store the size
 */
void get_board_size(FILE *fptr, int *size) {      
    char *line1 = NULL;
    size_t len = 0;
    if ( getline(&line1, &len, fptr) == -1 ) {
        printf("Error reading the input file.\n");
        exit(1);
    }

    char *size_chars = NULL;
    size_chars = strtok(line1, DELIM);
    *size = atoi(size_chars);
}

/* This is a helper method for valid_board. This checks the rows specifically
 * to see if each row is distict, the right size, and fits all the criteria.
 * If the rows do not fit the criteria, this returns 1, making the board say
 * "invalid."
 *
 * PRE-CONDITION #1: has to have a 2D array created in main
 * PRE-CONDITION #2: size and board loaded from user
 *
 * board: 2D array of the gameboard 
 * size: int of the size of the board 
 *
 * return: 0 if board is invalid 
 */

int rows(int **board, int size){
  //int variables for throughout method 
  int row, col, check, duplicate,i; 

  //loop through and get each element individually, store in check
  for(row = 0; row < size; row++)
  {
	for(col = 0; col < size; col++)
	{
	   check = *(*(board + col) + row);
	   //if check is greater than size, invalid 
	   if(check <= size)
	   {
		//continue is check == 0
		if(check != 0)
		{
		duplicate = 0;
		for(i = 0; i < size; i++)
		{
		   //count the duplicates
		   if(*(*(board + i) + row) == check)
		   {
			duplicate++;
		   }
		}
		//if theres duplicates, invalid 
		if(duplicate > 1)
		{
		   //invalid 
		   return 0;
		}
	       }
	   }
	   else
	      {
		//invalid
		return 0;
	      }
	}
   }
	return 1;	
}


/* This is another helper method for valid_board. This checks the columns
 * specifically to see if each columns is distict, the right size, and fits 
 * all the criteria. If the columns do not fit the criteria, this returns
 * 1, making the board say "invalid."
 *
 * PRE-CONDITION #1: has to have a 2D array created in main
 * PRE-CONDITION #2: size and board loaded from user
 *
 * board: 2D array of the gameboard 
 * size: int of the size of the board 
 *
 * return: 0 if board is invalid 
 */


int cols(int **board, int size)
{
   //int variables throughtout method 
   int col, row, check, duplicate, i;

   //loop through the 2D array, check the columns 
   for( col = 0; col < size; col++)
   {
	for(row = 0; row < size; row++)
	{
	   check  = *(*(board + col) + row);
	   //if the number is less than size, wrong number
	   if(check <= size)
	   {
	     //if check is 0, continue loop
	     if(check !=0)
	     {
		duplicate = 0;
		//if is what goes through each columns
		for(i = 0; i < size; i++)
		{
		   if (*(*(board + col) + i) == check)
		   {
		       //count the duplicates 
		       duplicate = duplicate + 1;
		   }
		}
		if(duplicate  > 1)
		{
		   //then invalid
		   return 0;
		}
	      }
	   }
	   //if the numbers are not within the size 
	   else
	   {
	      return 0;
	   }
	}
   }
   return 1;
}


/* TODO:
 * Returns 1 if and only if the board is in a valid Sudoku board state.
 * Otherwise returns 0.
 * 
 * A valid row or column contains only blanks or the digits 1-size, 
 * with no duplicate digits, where size is the value 1 to 9.
 * 
 * Note: p2A requires only that each row and each column are valid.
 * 
 * board: heap allocated 2D array of integers 
 * size:  number of rows and columns in the board
 */
int valid_board(int **board, int size) 
{
    //checks my helper methods, if they dont pass, return 1
    if(cols(board,size) != 1)
    {
	    return 0;
    }	
    if(rows(board,size) != 1){
	    return 0;
    }
    //if they pass, return 1 
    return 1;
}     
 

/* TODO: COMPLETE THE MAIN FUNCTION
 * This program prints "valid" (without quotes) if the input file contains
 * a valid state of a Sudoku puzzle board wrt to rows and columns only.
 *
 * argc: CLA count
 * argv: CLA value
 */
int main( int argc, char *argv[] ) {              
     
    // TODO: Check if number of command-line arguments is correct.
    
    // argc is the number of command line arguments 
    if(argc != 2)
    {
	printf("Usage: ./check_board <input_filename>\n"); //usage statement 
	exit(1);
    }
	

    // Open the file and check if it opened successfully.
    FILE *fp = fopen(*(argv + 1), "r");
    if (fp == NULL) {
        printf("Can't open file for reading.\n");
        exit(1);
    }

    // Declare local variables.
    int size;
    int *sizeptr;

    //initialize both sizeptr and size 
    sizeptr = &size;
   
    // TODO: Call get_board_size to read first line of file as the board size.
   
    //pointers to call get board size 
    get_board_size(fp, sizeptr);
    

    // TODO: Dynamically allocate a 2D array for given board size.

    //create the 2D array
    int **board_size;
    board_size = malloc(sizeof(int*) * size);
 

    //implement the array, fill  
    for(int i = 0; i < size; i++)
	{
	*(board_size + i) = malloc(sizeof(int) * size);
	}

    if(board_size == NULL)
	{
	printf("Nothing inside board");
	exit(1);
	}

    // Read the rest of the file line by line.
    // Tokenize each line wrt the delimiter character 
    // and store the values in your 2D array.
    char *line = NULL;
    size_t len = 0;
    char *token = NULL;
    for (int i = 0; i < size; i++) {

        if (getline(&line, &len, fp) == -1) {
            printf("Error while reading line %i of the file.\n", i+2);
            exit(1);
        }

        token = strtok(line, DELIM);
        for (int j = 0; j < size; j++) {
            // TODO: Complete the line of code below
            // to initialize your 2D array.
            /* ADD ARRAY ACCESS CODE HERE */
	//  *(board_size + j) = atoi(token);

	//pointer to pointer to boardsize at j
         *(*(board_size + i) + j)  = atoi(token);
	       	token = strtok(NULL, DELIM);
        }
    }

    // TODO: Call the function valid_board and print the appropriate
    //       output depending on the function's return value.

    //if valid board returns 1, is a valid board
    if(valid_board(board_size, size) == 1)
    {
	printf("valid\n");
    }
    else //if not 1, is invalid 
    {
	printf("invalid\n");
    }

    // TODO: Free all dynamically allocated memory

    for(int k = 0; k < size; k++)
    {
	//free each element in the array 
	free(*(board_size + k));
    }
   free(board_size);
   board_size = NULL;
  
    //Close the file.
    if (fclose(fp) != 0) {
        printf("Error while closing the file.\n");
        exit(1);
    }
} 

////////////////////////////////////////////////////////////////////////////////
// Main File:        myMagicSquare.c
// This File:        myMagicSquare.c
// Other Files:     
// Semester:         CS 354 Spring 2021
// Instructor:       deppeler
// 
// Discussion Group: group 6 
// Author:           Nicle Welsh 
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
///////////////////////////////////////////////////////////////////////////////
//
// Copyright 2020 Jim Skrentny
// Posting or sharing this file is prohibited, including any changes/additions.
// Used by permission, CS 354 Spring 2021, Deb Deppeler
//
////////////////////////////////////////////////////////////////////////////////
   

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structure that represents a magic square
typedef struct {
    int size;           // dimension of the square
    int **magic_square; // pointer to heap allocated magic square
} MagicSquare;

/* TODO:
 * Prompts the user for the magic square's size, reads it,
 * checks if it's an odd number >= 3 (if not display the required
 * error message and exit), and returns the valid number.
 */
int getSize() {
    int size;
    //gets the input from the user with scan f 
    printf("Enter magic square's size (odd integer >=3)\n");
    scanf("%i", &size);
    if(size % 2 == 0) //then input is even
    {
        printf("Magic square size must be odd.\n");
        exit(1);
    }
    if(size < 3) //then input is too small
    {
        printf("Magic square size must be >= 3.\n");
        exit(1);
    }
    
    //return valid size 
    return size;   
} 
   
/* TODO:
 * Makes a magic square of size n using the alternate 
 * Siamese magic square algorithm from assignment and 
 * returns a pointer to the completed MagicSquare struct.
 *
 * I do the version where it goes up and over to the right
 *
 * n the number of rows and columns
 */
MagicSquare *generateMagicSquare(int n) {
  int size = n;
   
   //create and allocate the 2D array 
   int **board;
   board = malloc(sizeof(int*)*size);
   
   //checks return value of malloc
   if(board == NULL)
	{
	printf("Nothing inside board\n");
	exit(1);
	}

   for(int i = 0; i < size; i++)
   {
	*(board + i) = malloc(sizeof(int) * size);
	if(*(board + i) == NULL)
	{
	   printf("Nothing inside board\n");
	   exit(1);
	}
	// *(board + i) =(MagicSquare *) malloc(sizeof(MagicSquare) * size);
   }   

      //row and col variables to be used throughout the method
  int row;
  int col; 
 
   //set all values to 0
   //later i use the 0 values to check if the place is used yet
   for (row = 0; row < size; row++) {
        for (col = 0; col < size; col++){
	*(*(board + row) + col) = 0;
	}

    }  
  
   // starting position at middle col top row
    row = 0;
    col = size / 2;

    //from 1 to n^2, place every number 
    for (int i = 1; i <= size*size; i++) {
	 //element is the number 1 to n^2
 	*(*(board + row)+col) = i;
	
	//move row up and col right  
	//nrow/col means next value for row or col
        int nrow = (row - 1 + size) % size;
        int ncol = (col + 2) % size;

	//if there is already a number there 
	if(*(*(board+nrow)+ncol) > 0){
            row = (row + 1) % size;
        }

	//otherwise go to next value to place  
        else {
            row = nrow;
            col = ncol;
        }
    
    }
    //i used this to check the values     
    /*
	for(int i = 0; i < size; i++){
	for(int j = 0; j < size; j++){
	   printf("%i, ",*(*(board+i)+j));
	}
	}
	printf("\n");
    */
    MagicSquare *square = malloc(sizeof(MagicSquare));
    
    //set the struct properties 
    square->magic_square = board;
    square->size = size;

     //checks return value of malloc
    if(square == NULL)
	{
	printf("Nothing inside board");
	exit(1);
	}
 
    //return pointer to a board 
    return square;  
} 

/* TODO:  
 * Opens a new file (or overwrites the existing file)
 * and writes the square in the specified format.
 *
 * magic_square the magic square to write to a file
 * filename the name of the output file
 */
void fileOutputMagicSquare(MagicSquare *magic_square, char *filename) {

    //creating the magic square board to print to the file 
    // MagicSquare *board;
    //board = (MagicSquare *)malloc(sizeof(MagicSquare));
    
    //copy the contents  
    int **board = magic_square->magic_square;

    //board-> size= magic_square->size; 
    //board->magic_square = magic_square->magic_square; 
    int size = magic_square->size;

   //checks return value of malloc
   if(board == NULL)
	{
	printf("Nothing inside board");
	exit(1);
	}

/*
    FILE *fp;
   
    fp = fopen(filename, "r");
    
    if (fp == NULL) 
    {
        printf("ERROR: cannot read \n");
        exit(1);
    }
*/
   //open the file and write to it 
    FILE *ofp = fopen(filename, "w");
    if (ofp == NULL) 
    {
        printf("ERROR: cannot write \n");
       // fclose(fp);
	exit(1);
    }

   fprintf(ofp, "%i\n", size);

    //print every element of the square to the file  
   for(int i = 0; i < size; i++)
    {
	for(int j = 0; j < size; j++ )
	{
	   fprintf(ofp, "%d", *(*(board+ i)+j));
	   //makes the new lines and adds commas 
	   if((j+1) % size == 0){
		fprintf(ofp, "\n");
	    }else{
		fprintf(ofp, ",");
	    }   
	}
    }
    
  //close the file 
   if (/*fclose(fp) != 0 || */fclose(ofp) != 0) {
        printf("Error while closing the file.\n");
        exit(1);
    }
  }

/* TODO:
 * Generates a magic square of the user specified size and
 * output the quare to the output filename
 */
int main(int argc, char *argv[]) {
    // TODO: Check input arguments to get output filename

    if(argc != 2)
    {
	printf("Usage: ./myMagicSquare <output_filename>\n");
	exit(1);
    }
    // TODO: Get magin square's size from user
 
   //size variable calls get size method to get size from user  
   int size;  
   size = getSize();
   
   // TODO: Generate the magic square
    
    MagicSquare *board;
    board = generateMagicSquare(size);
   
    
    // TODO: Output the magic square


   //get name of the file from user  
   char* fileName = *(argv+1);

   fileOutputMagicSquare(board, fileName);
   //fileOutputMagicSquare(generateMagicSquare(size), fileName); 
   

   //free here 
   for(int i = 0; i < size; i++)
	free(*(board->magic_square + i));
   //now free the magic square board and the board 
   free(board -> magic_square);
   free(board);
   board = NULL;

    return 0;
} 

                                                         
//		s21		myMagicSquare.c      


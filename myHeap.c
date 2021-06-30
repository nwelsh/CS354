////////////////////////////////////////////////////////////////////////////////
// Main File:        myHeap.c
// This File:        myHeap.c
// Other Files:      none
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
///////////////////////////////////////////////////////////////////////////////
//
// Copyright 2020-2021 Deb Deppeler based on work by Jim Skrentny
// Posting or sharing this file is prohibited, including any changes/additions.
// Used by permission Fall 2020, CS354-deppeler
//
///////////////////////////////////////////////////////////////////////////////

// DEB'S PARTIAL SOLUTION FOR SPRING 2021 DO NOT SHARE
 
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <stdio.h>
#include <string.h>
#include "myHeap.h"
#include <limits.h>

/* 
 * Information that says if a block is free or allocated and if prev is 
 * free or allocated. FREE_FREE means prev is free and current is free. 
 * calculated by binary digits in spot of the words free or alloc. 
 * ALLOC means 1 and FREE means 0
 * 
 */
 const int FREE_FREE = 0;
 const int FREE_ALLOC = 1;
 const int ALLOC_FREE = 2;
 const int ALLOC_ALLOC = 3;
/*
 * This structure serves as the header for each allocated and free block.
 * It also serves as the footer for each free block but only containing size.
 */
typedef struct blockHeader {           

    int size_status;
    /*
     * Size of the block is always a multiple of 8.
     * Size is stored in all block headers and in free block footers.
     *
     * Status is stored only in headers using the two least significant bits.
     *   Bit0 => least significant bit, last bit
     *   Bit0 == 0 => free block
     *   Bit0 == 1 => allocated block
     *
     *   Bit1 => second last bit 
     *   Bit1 == 0 => previous block is free
     *   Bit1 == 1 => previous block is allocated
     * 
     * End Mark: 
     *  The end of the available memory is indicated using a size_status of 1.
     * 
     * Examples:
     * 
     * 1. Allocated block of size 24 bytes:
     *    Allocated Block Header:
     *      If the previous block is free      p-bit=0 size_status would be 25
     *      If the previous block is allocated p-bit=1 size_status would be 27
     * 
     * 2. Free block of size 24 bytes:
     *    Free Block Header:
     *      If the previous block is free      p-bit=0 size_status would be 24
     *      If the previous block is allocated p-bit=1 size_status would be 26
     *    Free Block Footer:
     *      size_status should be 24
     */
} blockHeader;         

/* Global variable - DO NOT CHANGE. It should always point to the first block,
 * i.e., the block at the lowest address.
 */
blockHeader *heapStart = NULL;     

/* Size of heap allocation padded to round to nearest page size.
 */
int allocsize;

/*
 * Additional global variables may be added as needed below
 */

 
/* 
 * Function for allocating 'size' bytes of heap memory.
 * Argument size: requested size for the payload
 * Returns address of allocated block (payload) on success.
 * Returns NULL on failure.
 *
 * This function must:
 * - Check size - Return NULL if not positive or if larger than heap space.
 * - Determine block size rounding up to a multiple of 8 
 *   and possibly adding padding as a result.
 *
 * - Use BEST-FIT PLACEMENT POLICY to chose a free block
 *
 * - If the BEST-FIT block that is found is exact size match
 *   - 1. Update all heap blocks as needed for any affected blocks
 *   - 2. Return the address of the allocated block payload
 *
 * - If the BEST-FIT block that is found is large enough to split 
 *   - 1. SPLIT the free block into two valid heap blocks:
 *         1. an allocated block
 *         2. a free block
 *         NOTE: both blocks must meet heap block requirements 
 *       - Update all heap block header(s) and footer(s) 
 *              as needed for any affected blocks.
 *   - 2. Return the address of the allocated block payload
 *
 * - If a BEST-FIT block found is NOT found, return NULL
 *   Return NULL unable to find and allocate block for desired size
 *
 * Note: payload address that is returned is NOT the address of the
 *       block header.  It is the address of the start of the 
 *       available memory for the requesterr.
 *
 * Tips: Be careful with pointer arithmetic and scale factors.
 */

void* myAlloc(int size) {    
     /*
     * pad is the padding to make a block a muiltple of 8
     * sizeBlock holds size, header, and padding of the block being inserted
     * currSize is the size status of the current block % 8 
     * t_size is the size status of the current block 
     */
   int pad, sizeBlock, currSize, t_size;
    //header holds the size of the block header (4)
   int header = sizeof(blockHeader);

    //check size before use, return null if incorrect 
   if(size < 0 || size > allocsize)
   {
     return NULL;
   }
    //determine block size and add padding
    //determine if block needs padding, if not a multiple of 8
   sizeBlock = (header + size) % 8;
    //if sizeBlock is already a muiltple of 8, do not pad
   if(sizeBlock != 0)
   {
       pad = 8 - sizeBlock; 
       sizeBlock = size + header + pad; //pad to make multiple of 8
   }
   else
   {
	sizeBlock = header + size;
   }

    //current is what the loops uses, the place in the heap     
   blockHeader *current = heapStart;
    //nextSmallest is set to a large number so if the number is smaller, it will set to that 
   int nextBest = INT_MAX; 
    //addSize holds the header + size + padding if necessary 
   int addSize = sizeBlock; //
   //first iteration, find exact fit or smallest, second iteration 
   for(int x = 0; x < 2; x++){
   if(x == 0)
   {
       //if current size status is 1, it is at the footer 
      while(current->size_status != FREE_ALLOC)
	    {	
      		currSize = (current ->size_status) % 8; 
		t_size = current->size_status;
		if(currSize == ALLOC_FREE || currSize == FREE_FREE) //if the header says block is free 
	        {	   
		    //this is an exact match 
      		   if((current->size_status) - currSize == addSize)
		   {
                   	//if size status is 2, previous is allocated, need to add 1
              		if((current->size_status) % 8 == ALLOC_FREE)
			{
			     current->size_status = addSize + ALLOC_FREE + FREE_ALLOC;
     	                }
                 	//if size status is 0, add only 1 
			else if ((current->size_status) % 8 == FREE_FREE)
			{
			     current->size_status = addSize + FREE_ALLOC;
       	                }
			//nextBlock is the next block and allows allocation to be correct
                 	blockHeader *nextBlock = (blockHeader*)((char*)current + t_size - currSize);
		  
		  	   //if next is 1, the end and dont add to size status 
		  	   if(nextBlock->size_status != 1)
			   {
			         nextBlock->size_status += ALLOC_FREE;
			   }
			   //this sets the address correctly if it is not 0 already  
			   while((int)(current) % 8 != 0)
                  	   {
			        current++;
	                   }
                	return current;		    
                   }

	    	   if(addSize < (current->size_status) - currSize)
      		   {
		        //this means too big, need to save the smallest addr for next loop  
		        if((current->size_status) - currSize <  nextBest)
		        {
    			     //sets nextSmallest to the smallest value 
			      nextBest = (current->size_status) - currSize;
		        }
		   }	
         }
		//increment the current block
	     current = (blockHeader*)((char*)current + t_size - currSize); 
	   }
    }
   	//this is the second iteration, to find the best fit
   	else if(x==1) 
	{
	   current = heapStart; 
	   //reset the current and all elements to the beginning
	
	   //while current is not at the end, this is fairly the same as the before loop
	   while(current->size_status !=1)
	   {
		currSize = (current->size_status) % 8;
		t_size = current->size_status;
	    	
		if((currSize == ALLOC_FREE || currSize == FREE_FREE))
                {
	    	    if((current->size_status) - currSize == nextBest)
		    {
		   	 //if prev is allocated, add 2 for prev and 1 for new alloc 
		   	 if((current->size_status) % 8 == ALLOC_FREE)
			 {
			     current->size_status = addSize + ALLOC_FREE + FREE_ALLOC;
		   	 }
   	                 //if prev is not allocated, just add 1
		   	 else if ((current->size_status) % 8 == FREE_FREE)
			 {
			     current->size_status = addSize + FREE_ALLOC;
			 }
		   
		   	 //this is the free block: nextBest is getting split, 
		  	 int newFreeSize = nextBest - addSize;
		   	 blockHeader *newFree = (blockHeader*)((void*)current + addSize);
		   
		   	 //if the previous is allocated, add 2
			 if(newFree->size_status != FREE_ALLOC)
			 {
		   	     newFree->size_status = newFreeSize + ALLOC_FREE;
  	                 }
			 
			 while((int)(current) % 8 != 0)
                	 {
			      current++;
	                 }
		   	 return current;
		     }
        	}
	   	current = (blockHeader*)((char*)current + t_size - currSize); 
	    }
  	}	 
    }
    return NULL; //if there is failure
}  
/* 
 * Function for freeing up a previously allocated block.
 * Argument ptr: address of the block to be freed up.
 * Returns 0 on success.
 * Returns -1 on failure.
 * This function should:
 * - Return -1 if ptr is NULL.
 * - Return -1 if ptr is not a multiple of 8.
 * - Return -1 if ptr is outside of the heap space.
 * - Return -1 if ptr block is already freed.
 * - Update header(s) and footer as needed.
 */                    
//part b 
int myFree(void *ptr) {    
   //create a block header of ptr 
   blockHeader *toFree = ptr; 
   //size status of ptr
   int freeHdr = toFree->size_status;
   //same use as in myAlloc
   int t_size, currSize;
  
   //check for all reasons of failure 
   if(ptr == NULL  || (((int)ptr) % 8) != 0 || freeHdr > allocsize || (freeHdr & 1) != 0)
   {
	return -1;
   }

   //set current to heapstart
   blockHeader *current = heapStart;

   //this is the same loop as myAlloc
   while (current->size_status != 1) 
   {
	t_size = current->size_status;
	currSize = t_size % 8;

	//if current matches pointer, free that space
	if((int)current + sizeof(blockHeader) == (int)toFree)
	{
	    current->size_status -= FREE_ALLOC;
	    //create a next to update the header
	    blockHeader *next = (blockHeader*)((char*)current + t_size - currSize);
            //take away the 2 so prev free
	    next->size_status -= ALLOC_FREE;   
	}
	current = (blockHeader*)((char*)current + t_size - currSize);
   }
   //return 0 on success 
   return 0;
} 

/*
 * Function for traversing heap block list and coalescing all adjacent 
 * free blocks.
 *
 * This function is used for delayed coalescing.
 * Updated header size_status and footer size_status as needed.
 */
int coalesce() {

   //same variables as myAlloc
   blockHeader *current = heapStart;
   int t_size, currSize;

   //same loop as myAlloc
   while (current->size_status != 1) {
        t_size = current->size_status;
        currSize = (t_size) % 8;

	//make new next variable to check next 
	blockHeader *next = (blockHeader*)((char*)current + t_size - currSize); 
	//next's header
	int nextHdr = next->size_status;
	//next's status
	int chkNext = nextHdr % 8;
	
	//if current is free 
	if(currSize == FREE_FREE || currSize == ALLOC_FREE)
	{
	    if(chkNext == FREE_FREE)
	    {
		current->size_status = t_size + (nextHdr - chkNext);
		//recursively call coalesce incase there are 2+ in a row
		coalesce(); 	
	         }
	    //do the same 
           if(chkNext == ALLOC_FREE)
            {
		current->size_status = t_size + (nextHdr - chkNext - 2);
	    	coalesce();
	        }
        }
	//move through loop
   	current = (blockHeader*)((char*)current + t_size - currSize);
   }
	return 0;
}

 
/* 
 * Function used to initialize the memory allocator.
 * Intended to be called ONLY once by a program.
 * Argument sizeOfRegion: the size of the heap space to be allocated.
 * Returns 0 on success.
 * Returns -1 on failure.
 */      
//DO NOT CHANGE 
int myInit(int sizeOfRegion) {    
 
    static int allocated_once = 0; //prevent multiple myInit calls
 
    int pagesize;   // page size
    int padsize;    // size of padding when heap size not a multiple of page size
    void* mmap_ptr; // pointer to memory mapped area
    int fd;

    blockHeader* endMark;
  
    if (0 != allocated_once) {
        fprintf(stderr, 
        "Error:mem.c: InitHeap has allocated space during a previous call\n");
        return -1;
    }

    if (sizeOfRegion <= 0) {
        fprintf(stderr, "Error:mem.c: Requested block size is not positive\n");
        return -1;
    }

    // Get the pagesize
    pagesize = getpagesize();

    // Calculate padsize as the padding required to round up sizeOfRegion 
    // to a multiple of pagesize:wq
    padsize = sizeOfRegion % pagesize;
    padsize = (pagesize - padsize) % pagesize;

    allocsize = sizeOfRegion + padsize;

    // Using mmap to allocate memory
    fd = open("/dev/zero", O_RDWR);
    if (-1 == fd) {
        fprintf(stderr, "Error:mem.c: Cannot open /dev/zero\n");
        return -1;
    }
    mmap_ptr = mmap(NULL, allocsize, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);
    if (MAP_FAILED == mmap_ptr) {
        fprintf(stderr, "Error:mem.c: mmap cannot allocate space\n");
        allocated_once = 0;
        return -1;
    }
  
    allocated_once = 1;

    // for double word alignment and end mark
    allocsize -= 8;

    // Initially there is only one big free block in the heap.
    // Skip first 4 bytes for double word alignment requirement.
    heapStart = (blockHeader*) mmap_ptr + 1;

    // Set the end mark
    endMark = (blockHeader*)((void*)heapStart + allocsize);
    endMark->size_status = 1;

    // Set size in header
    heapStart->size_status = allocsize;

    // Set p-bit as allocated in header
    // note a-bit left at 0 for free
    heapStart->size_status += 2;

    // Set the footer
    blockHeader *footer = (blockHeader*) ((void*)heapStart + allocsize - 4);
    footer->size_status = allocsize;
  
    return 0;
} 
                  
/* 
 * Function to be used for DEBUGGING to help you visualize your heap structure.
 * Prints out a list of all the blocks including this information:
 * No.      : serial number of the block 
 * Status   : free/used (allocated)
 * Prev     : status of previous block free/used (allocated)
 * t_Begin  : address of the first byte in the block (where the header starts) 
 * t_End    : address of the last byte in the block 
 * t_Size   : size of the block as stored in the block header
 */                     
void dispMem() {     
 
    int counter;
    char status[6];
    char p_status[6];
    char *t_begin = NULL;
    char *t_end   = NULL;
    int t_size;

    blockHeader *current = heapStart;
    counter = 1;

    int used_size = 0;
    int free_size = 0;
    int is_used   = -1;

    fprintf(stdout, 
	"*********************************** Block List **********************************\n");
    fprintf(stdout, "No.\tStatus\tPrev\tt_Begin\t\tt_End\t\tt_Size\n");
    fprintf(stdout, 
	"---------------------------------------------------------------------------------\n");
  
    while (current->size_status != 1) {
        t_begin = (char*)current;
        t_size = current->size_status;
    
        if (t_size & 1) {
            // LSB = 1 => used block
            strcpy(status, "alloc");
            is_used = 1;
            t_size = t_size - 1;
        } else {
            strcpy(status, "FREE ");
            is_used = 0;
        }

        if (t_size & 2) {
            strcpy(p_status, "alloc");
            t_size = t_size - 2;
        } else {
            strcpy(p_status, "FREE ");
        }

        if (is_used) 
            used_size += t_size;
        else 
            free_size += t_size;

        t_end = t_begin + t_size - 1;
    
        fprintf(stdout, "%d\t%s\t%s\t0x%08lx\t0x%08lx\t%4i\n", counter, status, 
        p_status, (unsigned long int)t_begin, (unsigned long int)t_end, t_size);
    
        current = (blockHeader*)((char*)current + t_size);
        counter = counter + 1;
    }

    fprintf(stdout, 
	"---------------------------------------------------------------------------------\n");
    fprintf(stdout, 
	"*********************************************************************************\n");
    fprintf(stdout, "Total used size = %4d\n", used_size);
    fprintf(stdout, "Total free size = %4d\n", free_size);
    fprintf(stdout, "Total size      = %4d\n", used_size + free_size);
    fprintf(stdout, 
	"*********************************************************************************\n");
    fflush(stdout);

    return;  
} 


// end of myHeap.c (sp 2021)                                         







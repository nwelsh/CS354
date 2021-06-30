////////////////////////////////////////////////////////////////////////////////
// Main File:        csim.c
// This File:        csim.c
// Other Files:      none turning in
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

////////////////////////////////////////////////////////////////////////////////
//
// Copyright 2013,2019-2020, Jim Skrentny, (skrentny@cs.wisc.edu)
// Posting or sharing this file is prohibited, including any changes/additions.
// Used by permission, CS354-Spring 2021, Deb Deppeler (deppeler@cs.wisc.edu) 
//
////////////////////////////////////////////////////////////////////////////////

/**
 * csim.c:  
 * A cache simulator that can replay traces (from Valgrind) and 
 * output statistics for the number of hits, misses, and evictions.
 * The replacement policy is LRU.
 *
 * Implementation and assumptions:
 *  1. Each load/store can cause at most 1 cache miss plus a possible eviction.
 *  2. Instruction loads (I) are ignored.
 *  3. Data modify (M) is treated as a load followed by a store to the same
 *  address. Hence, an M operation can result in two cache hits, or a miss and a
 *  hit plus a possible eviction.
 */  

#include <getopt.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>
#include <limits.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>

/*****************************************************************************/
/* DO NOT MODIFY THESE VARIABLES *********************************************/

//Globals set by command line args.
int b = 0;  //number of block (b) bits
int s = 0;  //number of set (s) bits
int E = 0;  //number of lines per set ... associativity 

//Globals derived from command line args.
int B;  //block size in bytes: B = 2^b
int S;  //number of sets: S = 2^s

//Global counters to track cache statistics in access_data().
int hit_cnt = 0; 
int miss_cnt = 0; 
int evict_cnt = 0; 

//Global to control trace output
int verbosity = 0;  //print trace if set
/*****************************************************************************/
  
  
//Type mem_addr_t: Use when dealing with addresses or address masks.
typedef unsigned long long int mem_addr_t; 

//Type cache_line_t: Use when dealing with cache lines.
//TODO - COMPLETE THIS TYPE
typedef struct cache_line {                    
    char valid; 
    mem_addr_t tag;
    //Add a data member as needed by your implementation for LRU tracking.
    int LRU; //line with highest counter is LRU 
} cache_line_t; 

//Type cache_set_t: Use when dealing with cache sets
//Note: Each set is a pointer to a heap array of one or more cache lines.
typedef cache_line_t* cache_set_t; 
//Type cache_t: Use when dealing with the cache.
//Note: A cache is a pointer to a heap array of one or more sets.
typedef cache_set_t* cache_t; 

// Create the cache (i.e., pointer var) we're simulating.
cache_t cache;   

/**
 *  TODO - COMPLETE THIS FUNCTION
 * init_cache:

 * This function should allocate the data structures using malloc() to hold information 
 * about the sets and cache lines depending on the values of parameters S (S = 2s) and E.

 * Allocates the data structure for a cache (using malloc) with S sets and E lines per set.
 * Initializes all valid bits and tags with 0s.
 */                    
void init_cache() {

	//calculate S and B from s-bit and b-bit. S = 2^s 	
	S = pow(2, s);
	B = pow(2, b);
	
	//allocate cache global variable with size S (sets). now holds desired sets.  
	cache = malloc(sizeof(cache_set_t) * S);
	
	//check return value of malloc 
	if(cache == NULL){
		printf("Cache is empty");
		exit(1);
	}
	
	//loop through number of sets and add lines to each set
	for (int row = 0; row < S; row++) {
		//allocate each line into each set
		*(cache + row) = malloc(sizeof(cache_line_t) * E);
		
		if(*(cache + row) == NULL){
			printf("Cache is empty");
			exit(1);
		}

		//add information about each line 
		for (int col = 0; col < E; col++) {
			//set all line's data to 0 because they will initailized in access_data 
			(*(cache + row) + col)->valid = 0;
			(*(cache + row) + col)->tag = 0;
			(*(cache + row) + col)->LRU = 0;
		
		}
	}
}
//end of init_cache 

/**
 * TODO - COMPLETE THIS FUNCTION 
 * free_cache:
 * Frees all heap allocated memory used by the cache.
 */                    
void free_cache() {     

	//loop through number of sets
	for (int i = 0; i < S; i++) {
		//free each line in cache 
		free(*(cache + i));
	}
	//free global cache and set to null
	free(cache);
	cache = NULL;

}
   
   
/**
 * TODO - COMPLETE THIS FUNCTION 
 * access_data:
 * Simulates data access at given "addr" memory address in the cache.
 *
 * tracks the cache hit, misses, and evictions 
 *
 * I chose to do the second option in the write up: counter.
 *
 * If already in cache, increment hit_cnt
 * If not in cache, cache it (set tag), increment miss_cnt
 * If a line is evicted, increment evict_cnt
 */                    
void access_data(mem_addr_t addr) {      

	//initialize variables 
	mem_addr_t tag, set;
	int min, max, t, x;
	
	//line with the smallest counter is the LRU and is evicted 
	min = INT_MAX; 	//min used for eviction
	max = 0;	//max used to find LRU
	t = b + s;	


	tag = (addr >> t) & ((int)pow(2, 64 - t) - 1);   //set tag
	set = (addr >> b) & ((int)pow(2, s) - 1);  	 //set number of address 

	//set the LRU at the beginning with finding max and min 
	//check for max: search set and set the max value
	for (x = 0; x < E; x++) {
		//if the v bit is 1 and value is bigger than max (starting at 0)
		if ((*(cache + set) + x)->valid == 1 && (*(cache + set) + x)->LRU > max ) {
			max = (*(cache + set) + x)->LRU; //used to set the LRU
		}
	}
	//check for min: search set and set the min value 
	for (x = 0; x < E; x++) {
		//if valid bit is 1 and is bigger than min, set min 
		if ((*(cache + set) + x)->valid == 1 && (*(cache + set) + x)->LRU < min) {
			min = (*(cache + set) + x)->LRU; //used for eviction 
		}
	}


	//check for hit, miss and eviction 
	//check for hit: if hit, update hit count and LRU
	for (x = 0; x < E; x++) {
		//if v bit is 1 and the tag matches
		if ((*(cache + set) + x)->valid == 1 && (*(cache + set) + x)->tag == tag) {
			//set new LRU value 
			(*(cache + set) + x)->LRU = max + 1;
			hit_cnt++;
			return;
		}
	}

	//check miss: if miss (v bit = 0), update miss count and create new line at value 
	for ( x = 0; x < E; x++) {
		//if v bit is 0, it is a miss 
		if ((*(cache + set) + x)->valid == 0) {
			//create the line, add the tag, set v bit to 1, add val to LRU, increment miss count 
			(*(cache + set) + x)->tag = tag;
			(*(cache + set) + x)->valid = 1;
			(*(cache + set) + x)->LRU = max + 1;
			miss_cnt++;
			return;
		}
	}

	//evict the line: go to min (LRU)
	for (x = 0; x < E; x++) { 
		if ((*(cache + set) + x)->LRU == min){
			//set tag, v bit, and reset LRU to value 
			(*(cache + set) + x)->tag = tag;
			(*(cache + set) + x)->valid = 1;
			(*(cache + set) + x)->LRU = max + 1;
			//update miss count and evict count 
			miss_cnt++;
			evict_cnt++;
			return;
		}
	}
}
  
  
/**
 * TODO - FILL IN THE MISSING CODE
 * replay_trace:
 * Replays the given trace file against the cache.
 *
 * Reads the input trace file line by line.
 * Extracts the type of each memory access : L/S/M
 * TRANSLATE each "L" as a load i.e. 1 memory access
 * TRANSLATE each "S" as a store i.e. 1 memory access
 * TRANSLATE each "M" as a load followed by a store i.e. 2 memory accesses 
 */                    
void replay_trace(char* trace_fn) {           
    char buf[1000];   
    mem_addr_t addr = 0; 
    unsigned int len = 0; 
    FILE* trace_fp = fopen(trace_fn, "r");  

    if (!trace_fp) { 
        fprintf(stderr, "%s: %s\n", trace_fn, strerror(errno)); 
        exit(1);    
    }

    while (fgets(buf, 1000, trace_fp) != NULL) {
        if (buf[1] == 'S' || buf[1] == 'L' || buf[1] == 'M') {
            sscanf(buf+3, "%llx,%u", &addr, &len); 
      
            if (verbosity)
                printf("%c %llx,%u ", buf[1], addr, len); 

            // TODO - MISSING CODE
            // GIVEN: 1. addr has the address to be accessed
            //        2. buf[1] has type of acccess(S/L/M)
            // call access_data function here depending on type of access
            
	    //if the address is type S L 
	    if (buf[1] == 'S' || buf[1] == 'L') {
                access_data(addr);
	    }   

	    //if address is type M, call twice because "load followed by a store"
	    if (buf[1] == 'M') {
		access_data(addr); //load
		access_data(addr); //store
	    }

            if (verbosity)
                printf("\n"); 
        }
    }

    fclose(trace_fp); 
}  
  
  
/**
 * print_usage:
 * Print information on how to use csim to standard output.
 */                    
void print_usage(char* argv[]) {                 
    printf("Usage: %s [-hv] -s <num> -E <num> -b <num> -t <file>\n", argv[0]); 
    printf("Options:\n"); 
    printf("  -h         Print this help message.\n"); 
    printf("  -v         Optional verbose flag.\n"); 
    printf("  -s <num>   Number of s bits for set index.\n"); 
    printf("  -E <num>   Number of lines per set.\n"); 
    printf("  -b <num>   Number of b bits for block offsets.\n"); 
    printf("  -t <file>  Trace file.\n"); 
    printf("\nExamples:\n"); 
    printf("  linux>  %s -s 4 -E 1 -b 4 -t traces/yi.trace\n", argv[0]); 
    printf("  linux>  %s -v -s 8 -E 2 -b 4 -t traces/yi.trace\n", argv[0]); 
    exit(0); 
}  
  
  
/**
 * print_summary:
 * Prints a summary of the cache simulation statistics to a file.
 */                    
void print_summary(int hits, int misses, int evictions) {                
    printf("hits:%d misses:%d evictions:%d\n", hits, misses, evictions); 
    FILE* output_fp = fopen(".csim_results", "w"); 
    assert(output_fp); 
    fprintf(output_fp, "%d %d %d\n", hits, misses, evictions); 
    fclose(output_fp); 
}  
  
  
/**
 * main:
 * Main parses command line args, makes the cache, replays the memory accesses
 * free the cache and print the summary statistics.  
 */                    
int main(int argc, char* argv[]) {                      
    char* trace_file = NULL; 
    char c; 
    
    // Parse the command line arguments: -h, -v, -s, -E, -b, -t 
    while ((c = getopt(argc, argv, "s:E:b:t:vh")) != -1) {
        switch (c) {
            case 'b':
                b = atoi(optarg); 
                break; 
            case 'E':
                E = atoi(optarg); 
                break; 
            case 'h':
                print_usage(argv); 
                exit(0); 
            case 's':
                s = atoi(optarg); 
                break; 
            case 't':
                trace_file = optarg; 
                break; 
            case 'v':
                verbosity = 1; 
                break; 
            default:
                print_usage(argv); 
                exit(1); 
        }
    }

    //Make sure that all required command line args were specified.
    if (s == 0 || E == 0 || b == 0 || trace_file == NULL) {
        printf("%s: Missing required command line argument\n", argv[0]); 
        print_usage(argv); 
        exit(1); 
    }

    //Initialize cache.
    init_cache(); 

    //Replay the memory access trace.
    replay_trace(trace_file); 

    //Free memory allocated for cache.
    free_cache(); 

    //Print the statistics to a file.
    //DO NOT REMOVE: This function must be called for test_csim to work.
    print_summary(hit_cnt, miss_cnt, evict_cnt); 
    return 0;    
}  


// end csim.c


#include <stdio.h> // for printf fprintf fgets
#include <stdlib.h> // for malloc
#include <string.h> // for strlen

int main(){
  char *str = malloc(50);  /* allocate a poniter to a char, 50 chars */
  printf("Enter your name");
  
  if(fgets (str, 50, stdin) == NULL)
     fprintf(stderr, "Error reading input. \n");
  
  int len = strlen(str);
  if(str[len - 1] == '\n'){
    str[len - 1] = '\0' /* make end */
    }
  
  printf("your name is ", str);
  
  return 0;
}

#include <stdio.h> // for printf fprintf fgets
#include <stdlib.h> // for malloc
#include <string.h> // for strlen

int main(){

        int *a = malloc(sizeof(int) * 4); //4 bytes, on the heap
        *(a) = 0;
        *(a + 1) = 1;
        *(a + 2) = 2;
        *(a + 3) = 3;

        int *b = malloc(sizeof(int)* 4);
        *(b) = 0;
        *(b + 1) = 1;
        *(b + 2) = 2;
        *(b + 3) = 4;

        for(int i = 0; i < sizeof(a); i++){
           if(*(a + i) != *(b + i)){
                printf("the %ith element is not the same", i); //for some reason it says third and sixth element
           }
        }
        return 0;

}

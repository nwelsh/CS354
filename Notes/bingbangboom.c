#include <stdio.h>

int bing(int x) {
x = x + 3;
printf("bing %d\n", x);
return x - 1;
}

int bang(int x) {
x = x + 2;
x = bing(x);
printf("BanG %d\n", x);
return x - 2;
}

int main(void) {
int x = 1;
bang(x);
printf("BOOM %d\n", x);
return 0;
}

/* 
Bing 6
bang 5 
boom 1 because didnt do x=
*/

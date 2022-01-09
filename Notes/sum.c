#include <stdio.h>
int main(){
        int num1, num2, sum;

        printf( "Please input two integers: ");
        scanf("%i %i", &num1, &num2);

        sum = num1 + num2;

        printf("%i + %i = %i \n", num1, num2, sum);

        return 0;
}

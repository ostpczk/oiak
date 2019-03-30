#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

struct Double_number {
   bool  sign;
   bool  exponent[11];
   bool  fraction[52];
};

int main()
{

    double op_a, op_b, result;
    printf("SUMOWANIE:\nWpisz pierwsza liczbe : ");
    scanf("%lf", &op_a);
    printf("\nWpisz druga liczbe:");
    scanf("%lf", &op_b);

    result = op_a + op_b;
    printf("\nWynik to: %lf", result);

    return 0;
}

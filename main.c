#include <stdio.h>
#include <stdlib.h>

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

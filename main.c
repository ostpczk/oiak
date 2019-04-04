#include <stdio.h>
#include <stdlib.h>

struct Double_number {
   int sign;
   int exponent; // 11 bitów
   int fraction; // maks. 52 bity
}d0, d1;

//assumes little endian
/*
void printBits(size_t const size, void const * const ptr)
{
    unsigned char *b = (unsigned char*) ptr;
    unsigned char byte;
    int i, j;

    for (i=size-1;i>=0;i--)
    {
        for (j=7;j>=0;j--)
        {
            byte = (b[i] >> j) & 1;
            printf("%u", byte);
        }
    }
    puts("");
}*/

extern int* add(double* o_1, double* o_2);

int main()
{

    double op_a = 0;
    double op_b = 0;
    int* result = 0;
    printf("SUMOWANIE:\nWpisz pierwsza liczbe : ");
    scanf("%lf", &op_a);
    printf("\nWpisz druga liczbe:");
    scanf("%lf", &op_b);

    double* p_a;
    double* p_b;

    p_a = &op_a;
    p_b = &op_b;

    result = add(p_a, p_b);

    printf("\nWynik to:"); // segmentation fault na wywołaniu tej funkcji - dlaczego?

    for(int i = 0; i < 64; i++)
    {
        printf("%d", result[i]);
    }

    return 0;
}

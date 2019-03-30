#include <stdio.h>
#include <stdlib.h>

struct Double_number {
   int sign;
   int exponent; // 11 bitów
   int fraction; // maks. 52 bity
}d0, d1;

//assumes little endian
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
}

extern int add(double o_1, double o_2);

int main()
{


    d0.sign = 0b0;
    d1.sign = 0b0;
    d0.exponent = 0b11000010000;
    d1.exponent = 0b11000000000;
    d0.fraction = 0b0000000000000000000000000000000000000000000000000000;
    d1.fraction = 0b0000000000000000000000000000000000000000000000000000;




    double op_a, op_b, result;
    printf("SUMOWANIE:\nWpisz pierwsza liczbe : ");
    scanf("%lf", &op_a);
    printf("\nWpisz druga liczbe:");
    scanf("%lf", &op_b);

    if (op_a >= 0) d0.sign= 0;
    else d0.sign=1;


    int size= 54;
    printBits(sizeof(op_a), &op_a);



    result = add(op_a, op_b);

    //result = op_a + op_b;
    printf("\nWynik to: %lf", result);

    return 0;
}

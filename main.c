#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include <string.h>
#define FLT_RADIX 2


int fractional_part_as_int(double number, int number_of_decimal_places) {
    double frac = number - (int) number;
    return round(frac*pow(10,number_of_decimal_places));
}

int get_number_of_digit(double num)
{
    int count = 0;
    while(num != 0)
    {
        num = num * 10;
        count = count + 1;
        num = num - (int)num;
    };
    return count;
}

union fi_t // zaawansowana metoda ekstrakcji bitow reprezentacji
{
    unsigned int i;
    float f;
};

uint64_t get_double_bits(double number) {
    uint64_t bits;
    memcpy(&bits, &number, sizeof number);
    return bits;
}

//extern int* add(double* o_1, double* o_2);

int main()
{
    double first = 3.5;
    double second = 5.5;

    printf("Given the number %lf and %lf\n", first, second);

    int i = (int) (log2(first) + 0.01 ); // Czesc calkowita liczby. Dodanie 0.01 eliminuje bledy zaokraglenia
    double mix_1 = first/scalbn(1.0, i);
    printf("mix representation first %lf *  %d^%d\n", mix_1, 2, (int)(log2(first)+0.01));

    double f3 = floor(mix_1);  // czesc calkowita
    double f2 = mix_1-f3; // czesc ulamkowa
    printf("modf() makes %lf + %lf\n", f3, f2);

    printf("%d\n",get_number_of_digit(f2));
    int intFractional = fractional_part_as_int(f2,get_number_of_digit(f2));

    char binary[32];
    for(int i = 31; i >= 0; i--)
    {
        if(intFractional%2 == 0) binary[i] = '0';
        else binary[i] = '1';
        intFractional >>= 1;
    }
    binary[32] = '\0';
    printf("%s\n", binary);

    size_t cutoff = 0;
    while(cutoff < 32 && binary[cutoff] == '0')
    {
        cutoff++;
    }
    char fit_binary[32];
    for(int i = 0; i < 32-cutoff; i++)
    {
        fit_binary[i] = binary[cutoff+i];
    }
    fit_binary[32-cutoff] = '\0';
    printf("%s\n",fit_binary);


    //================
    double number = -300.6969;

    printf("%Podaj: liczbe do przekonwertowania:");
    scanf("%lf",&number);

    uint64_t display_number = get_double_bits(number);

    char sign;
    char exponent[11]; // 11 bitów
    exponent[11] = '\0';
    char fraction[52]; // maks. 52 bity
    fraction[52] = '\0';

    printf("%ld\n",display_number);

    for(int i = 51; i >= 0; i--)
    {
        if (display_number % 2 == 0) fraction[i] = '0';
        else fraction[i] = '1';
        display_number >>= 1;
    }

    for(int i = 10; i >= 0; i--)
    {
        if (display_number % 2 == 0) exponent[i] = '0';
        else exponent[i] = '1';
        display_number >>= 1;
    }


    if (display_number % 2 == 0) sign = '0';
    else sign = '1';



    printf("%c|%s|%s\n",sign,exponent,fraction);






    /////=======================================
/*
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

cod    p_b = &op_b;
    //result = add(p_a, p_b);
    //asm("movl $0, %eax");
    printf("\nWynik to:\n"); // segmentation fault na wywołaniu tej funkcji - dlaczego?

    for(int i = 0; i < 64; i++)
    {
        printf("%d", result[i]);
    }
*/
    return 0;
}

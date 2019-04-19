#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include <mmintrin.h>
#include <immintrin.h>
#include <xmmintrin.h>
#include <emmintrin.h>
#define FLT_RADIX 2


int main()
{

    char binary[32];
    binary[1] = '0'; // usuwanie tego psuje reprezentacje - dlaczego?


    //================

    double number;

    printf("Podaj pierwsza liczbe:");
    scanf("%lf",&number);

    union d64i_t { uint64_t integer_number; double double_number;} d64i;
    d64i.double_number = number;
    uint64_t display_number = d64i.integer_number;

    char sign1;
    char exponent1[11]; // 11 bitów
    exponent1[11] = '\0';
    char fraction1_1[20]; // mantysa podzielona na dwie części
    char fraction1_2[32]; // 20 + 32 bity
    fraction1_1[20] = '\0';
    fraction1_2[32] = '\0';

    printf("%ld\n",display_number);

    for(int i = 31; i >= 0; i--)
    {
        if (display_number % 2 == 0) fraction1_2[i] = '0';
        else fraction1_2[i] = '1';
        display_number >>= 1;
    }

    for(int i = 19; i >= 0; i--)
    {
        if (display_number % 2 == 0) fraction1_1[i] = '0';
        else fraction1_1[i] = '1';
        display_number >>= 1;
    }


    for(int i = 10; i >= 0; i--)
    {
        if (display_number % 2 == 0) exponent1[i] = '0';
        else exponent1[i] = '1';
        display_number >>= 1;
    }


    if (display_number % 2 == 0) sign1 = '0';
    else sign1 = '1';

    printf("%c|%s|%s %s\n",sign1,exponent1,fraction1_1,fraction1_2);

    printf("Podaj druga liczbe:");
    scanf("%lf",&number);

    d64i.double_number = number;
    display_number = d64i.integer_number;

    char sign2;
    char exponent2[11]; // 11 bitów
    exponent2[11] = '\0';
    char fraction2_1[20]; // mantysa podzielona na dwie części
    char fraction2_2[32]; // 20 + 32 bity
    fraction2_1[20] = '\0';
    fraction2_2[32] = '\0';


    printf("%ld\n",display_number);

    for(int i = 31; i >= 0; i--)
    {
        if (display_number % 2 == 0) fraction2_2[i] = '0';
        else fraction2_2[i] = '1';
        display_number >>= 1;
    }

    for(int i = 19; i >= 0; i--)
    {
        if (display_number % 2 == 0) fraction2_1[i] = '0';
        else fraction2_1[i] = '1';
        display_number >>= 1;
    }

    for(int i = 10; i >= 0; i--)
    {
        if (display_number % 2 == 0) exponent2[i] = '0';
        else exponent2[i] = '1';
        display_number >>= 1;
    }


    if (display_number % 2 == 0) sign2 = '0';
    else sign2 = '1';

    printf("%c|%s|%s %s\n",sign2,exponent2,fraction2_1, fraction2_2);


    union{ __m64 mantissas; int addends[2]} ma1; // 12|20|32|12|20|32 bity
    union{ __m64 mantissas; int addends[2]} ma2; // 12|20|32|12|20|32 bity

    ma1.addends[0] = 0;
    for(int i = 0; i < 20; i++){
        if(fraction1_1[i] == '1')
            ma1.addends[0] += 1 << (19-i);
    }

    ma1.addends[1] = 0;
    for(int i = 0; i < 32; i++){
        if(fraction1_2[i] == '1')
            ma1.addends[1] += 1 << (31-i);
    }
    ma2.addends[0] = 0;
    for(int i = 0; i < 20; i++){
        if(fraction2_1[i] == '1')
            ma2.addends[0] += 1 << (19-i);
    }

    ma2.addends[1] = 0;
    for(int i = 0; i < 32; i++){
        if(fraction2_2[i] == '1')
            ma2.addends[1] += 1 << (31-i);
    }
    printf("%d %d %d %d", ma1.addends[0], ma1.addends[1], ma2.addends[0], ma2.addends[1]);

    __m64 a = ma1.mantissas;
    __m64 b = ma2.mantissas;


    a = _mm_add_pi32(a,b);
    return 0;

}

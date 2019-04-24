#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include <mmintrin.h>
#include <immintrin.h>
#include <xmmintrin.h>
#include <emmintrin.h>
#define FLT_RADIX 2

struct xDOUBLE {
char sign;

char exponent[12];

uint16_t exponent_int;

char fraction1[21]; // mantysa podzielona na dwie części
char fraction2[33]; // 20 + 32 bity

uint32_t fraction1_int;
uint32_t fraction2_int;

} op1, op2, op3; // "operand"


int main()
{

    op1.exponent[11] = '\0';    // inicjalizacje
    op1.fraction1[20] = '\0';   // znakow
    op1.fraction2[32] = '\0';   // konca

    op2.exponent[11] = '\0';    // ciagu
    op2.fraction1[20] = '\0';   // znakow
    op2.fraction2[32] = '\0';   // !!!

    op3.exponent[11] = '\0';
    op3.fraction1[20] = '\0';
    op3.fraction2[32] = '\0';


    //================

    double input;

    printf("Podaj pierwsza liczbe:");
    scanf("%lf",&input);

    union d64i_t { uint64_t integer_number; double double_number;} d64i;

    d64i.double_number = input;
    uint64_t display_number = d64i.integer_number;

    printf("%ld\n",display_number); // debug

    for(int i = 31; i >= 0; i--)
    {
        if (display_number % 2 == 0) op1.fraction2[i] = '0';
        else op1.fraction2[i] = '1';
        display_number >>= 1;
    }

    for(int i = 19; i >= 0; i--)
    {
        if (display_number % 2 == 0) op1.fraction1[i] = '0';
        else op1.fraction1[i] = '1';
        display_number >>= 1;
    }

    op1.exponent_int = 0;
    for(int i = 10; i >= 0; i--)
    {
        if (display_number % 2 == 0) op1.exponent[i] = '0';
        else
        {
            op1.exponent[i] = '1';
            op1.exponent_int += 1 << (10-i);
        }
        display_number >>= 1;
    }


    if (display_number % 2 == 0) op1.sign = '0';
    else op1.sign = '1';

    printf("%c|%s|%s %s\n",op1.sign,op1.exponent,op1.fraction1,op1.fraction2);

    printf("Podaj druga liczbe:");
    scanf("%lf",&input);

    d64i.double_number = input;
    display_number = d64i.integer_number;

    printf("%ld\n",display_number); // debug

    for(int i = 31; i >= 0; i--)
    {
        if (display_number % 2 == 0) op2.fraction2[i] = '0';
        else op2.fraction2[i] = '1';
        display_number >>= 1;
    }

    for(int i = 19; i >= 0; i--)
    {
        if (display_number % 2 == 0) op2.fraction1[i] = '0';
        else op2.fraction1[i] = '1';
        display_number >>= 1;
    }

    op2.exponent_int = 0;
    for(int i = 10; i >= 0; i--)
    {
        if (display_number % 2 == 0) op2.exponent[i] = '0';
        else
        {
            op2.exponent[i] = '1';
            op2.exponent_int += 1 << (10-i);
        }
        display_number >>= 1;
    }


    if (display_number % 2 == 0) op2.sign = '0';
    else op2.sign = '1';

    printf("%c|%s|%s %s\n",op2.sign,op2.exponent,op2.fraction1, op2.fraction2);


    // Konwersja do rejestrów i dodawanie mantys.

    union{ __m64 mantissas; uint32_t addends[2];} ma1, ma2, ma3; // 12|20|32|12|20|32 bity


    ma1.addends[0] = 0;
    for(uint32_t i = 0; i < 20; i++){
        if(op1.fraction1[i] == '1')
            ma1.addends[0] += (1 << (20-i));
    }

    ma1.addends[1] = 0;
    for(uint32_t i = 0; i < 32; i++){
        if(op1.fraction2[i] == '1')
            ma1.addends[1] += (1 << (32-i));
    }
    ma2.addends[0] = 0;
    for(uint32_t i = 0; i < 20; i++){
        if(op2.fraction1[i] == '1')
            ma2.addends[0] += (1 << (20-i));
    }

    ma2.addends[1] = 0;
    for(uint32_t i = 0; i < 32; i++){
        if(op2.fraction2[i] == '1')
            ma2.addends[1] += (1 << (32-i));
    }

    printf("%u %u %u %u\n", ma1.addends[0], ma1.addends[1], ma2.addends[0], ma2.addends[1]);

    __m64 a, b, c;

    a = ma1.mantissas;
    b = ma2.mantissas;

    if(op1.exponent_int < op2.exponent_int)
    {
        op3.exponent_int = op2.exponent_int;
        op3.sign = op2.sign;
        int diff = op2.exponent_int - op1.exponent_int;
        a = _m_psrlqi(a,diff);    }
    else
    {
        op3.exponent_int = op1.exponent_int;
        op3.sign = op1.sign;
        int diff = op1.exponent_int - op2.exponent_int;
        b = _m_psrlqi(b,diff);
    }


    if(op1.sign == op2.sign)
    {
    c = _mm_add_pi32(a,b); // sumujemy wartosci obu mantys
    }
    else
    {
    c = _mm_sub_pi32(a,b);
    }

    if((ma3.addends[0] % (1 << 22)) >= (1 << 21))
    {
    op3.exponent_int++;
    c = _m_psrlqi(c,1); // przesuniecie w prawo o 1 pozycje, wypelnienie zerami
    }

    ma3.mantissas = c;
    uint32_t decode_number = ma3.addends[1];
    for (int i = 0; i < 32; i++)
    {
        if( decode_number % 2 == 0) op3.fraction2[31-i] = '0';
        else op3.fraction2[31-i] = '1';
        decode_number >>= 1;
    }

    decode_number = ma3.addends[0];
    for (int i = 0; i < 20; i++)
    {
        if( decode_number % 2 == 0) op3.fraction1[19-i] = '0';
        else op3.fraction1[19-i] = '1';
        decode_number >>= 1;
    }

    decode_number = op3.exponent_int;
    for (int i = 0; i < 12; i++)
    {
        if( decode_number % 2 == 0) op3.exponent[10-i] = '0';
        else op3.exponent[10-i] = '1';
        decode_number >>= 1;
    }

    printf("%c|%s|%s %s\n",op3.sign,op3.exponent,op3.fraction1,op3.fraction2);

    return 0;

}

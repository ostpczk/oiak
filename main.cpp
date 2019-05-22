#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include <fenv.h>

#include "operations.h"

#define FLT_RADIX 2

#pragma STDC FENV_ACCESS on

double testdouble1, testdouble2, testdouble3, testdouble4;

uint64_t display_number;
d64i_t d64i;

void out(xDouble* op1, xDouble* op2, xDouble* op3)
{
    fflush(stdout);
/////////////// DRUkuj w postaci binarnej.

    display_number = op3->fraction2;
    for(int i = 31; i >= 0; i--)
    {
        if (display_number % 2 == 0) op3->fraction2_s[i] = '0';
        else op3->fraction2_s[i] = '1';
        display_number >>= 1;
    }

    display_number = op3->fraction1;
    for(int i = 19; i >= 0; i--)
    {
        if (display_number % 2 == 0) op3->fraction1_s[i] = '0';
        else op3->fraction1_s[i] = '1';
        display_number >>= 1;
    }

        char fraction0_s[13];
        fraction0_s[12] = '\0';

    for(int i = 11; i >= 0; i--)
    {
        if (display_number % 2 == 0) fraction0_s[i] = '0';
        else fraction0_s[i] = '1';
        display_number >>= 1;
    }


    display_number = op3->exponent;
    for(int i = 10; i >= 0; i--)
    {
        if (display_number % 2 == 0) op3->exponent_s[i] = '0';
        else op3->exponent_s[i] = '1';
        display_number >>= 1;
    }

    printf("Suma:\n");
    printf("%c|%s|%s %s\n",op3->sign,op3->exponent_s, op3->fraction1_s,op3->fraction2_s);
    //printf("%c|%s|(%s)%s %s\n",op3->sign,op3->exponent_s,fraction0_s, op3->fraction1_s,op3->fraction2_s);

    d64i.integer_number = 0;
    if (op3->sign == '1')
        d64i.integer_number += 1 << 11;

    d64i.integer_number += op3->exponent;
    d64i.integer_number <<= 20;
    d64i.integer_number += op3->fraction1;
    d64i.integer_number <<= 32;
    d64i.integer_number += op3->fraction2;

    testdouble4 = (double) d64i.double_number; //programistyczne woodo nie usowac tej linii

    printf("UZYSKANA WARTOSC: %.32e\n",d64i.double_number);
    printf("POZADANA WARTOSC: %.32e\n", testdouble3);
}



void in(xDouble* op1, xDouble* op2, xDouble* op3)
{
    double input;

    printf("Podaj pierwsza liczbe:");
    scanf("%lf",&input);

    testdouble1 = input;

    d64i.double_number = input;
    display_number = d64i.integer_number;

  // liczba jest "wyswietlana"
    // poprawnie nalezy teraz obliczac reszte z dzielenia przez 2 i zapisac do tabeli
    for(int i = 31; i >= 0; i--)
    {
        if (display_number % 2 == 0)
            op1->fraction2_s[i] = '0';
        else
        {
            op1->fraction2_s[i] = '1';
            op1->fraction2 += (1 << (31-i));
        }
        display_number >>= 1;
    }

    for(int i = 19; i >= 0; i--)
    {
        if (display_number % 2 == 0)
            op1->fraction1_s[i] = '0';
        else
        {
            op1->fraction1_s[i] = '1';
            op1->fraction1 += (1 << (19-i));
        }
        display_number >>= 1;
    }

    op1->exponent = 0;
    for(int i = 10; i >= 0; i--)
    {
        if (display_number % 2 == 0)
            op1->exponent_s[i] = '0';
        else
        {
            op1->exponent_s[i] = '1';
            op1->exponent += 1 << (10-i);
        }
        display_number >>= 1;
    }


    if (display_number % 2 == 0) op1->sign = '0';
    else op1->sign = '1';
    // drukuje na ekran pierwsza liczbew w formacie 1|11|52
    printf("%c|%s|%s %s\n",op1->sign,op1->exponent_s,op1->fraction1_s,op1->fraction2_s);

    printf("Podaj druga liczbe:");
    scanf("%lf",&input);

    testdouble2 = input;

    d64i.double_number = input;
    display_number = d64i.integer_number;

    for(int i = 31; i >= 0; i--)
    {
        if (display_number % 2 == 0) op2->fraction2_s[i] = '0';
        else
        {
            op2->fraction2_s[i] = '1';
            op2->fraction2 += (1 << 31-i);
        }
        display_number >>= 1;
    }

    for(int i = 19; i >= 0; i--)
    {
        if (display_number % 2 == 0) op2->fraction1_s[i] = '0';
        else
        {
            op2->fraction1_s[i] = '1';
            op2->fraction1 += (1 << 19-i); // TODO 10 czy 19?
        }
        display_number >>= 1;
    }

    op2->exponent = 0;
    for(int i = 10; i >= 0; i--)
    {
        if (display_number % 2 == 0) op2->exponent_s[i] = '0';
        else
        {
            op2->exponent_s[i] = '1';
            op2->exponent += 1 << (10-i);
        }
        display_number >>= 1;
    }


    if (display_number % 2 == 0) op2->sign = '0';
    else op2->sign = '1';

    printf("%c|%s|%s %s\n",op2->sign,op2->exponent_s,op2->fraction1_s, op2->fraction2_s);
}


int menu()
{
int picked_option=0;
printf("\nCo byc chcial uczynic\n");
printf("1-Dodawanie\n");
printf("2-Odejmowanie\n");
printf("3-Mnozenie\n");
printf("6-Test\n");

scanf("%d", &picked_option);
return picked_option;
}


int main()
{

    fesetround(FE_TOWARDZERO);
    xDouble op1;
    xDouble op2;
    xDouble op3;

    op1.exponent = 0;
    op1.fraction1 = 0;
    op1.fraction2 = 0;

    op2.exponent = 0;
    op2.fraction1 = 0;
    op2.fraction2 = 0;

    op3.exponent = 0;
    op3.fraction1 = 0;
    op3.fraction2 = 0;


    op1.exponent_s[11] = '\0';    // inicjalizacje
    op1.fraction1_s[20] = '\0';   // znakow
    op1.fraction2_s[32] = '\0';   // konca

    op2.exponent_s[11] = '\0';    // inicjalizacje
    op2.fraction1_s[20] = '\0';   // znakow
    op2.fraction2_s[32] = '\0';   // konca

    op3.exponent_s[11] = '\0';    // inicjalizacje
    op3.fraction1_s[20] = '\0';   // znakow
    op3.fraction2_s[32] = '\0';   // konca

    xDouble* op1_p = &op1;
    xDouble* op2_p = &op2;
    xDouble* op3_p = &op3;



    switch(menu())
    {
    case 1: // dodawanie
    {
    in(op1_p,op2_p,op3_p);
    testdouble3 = testdouble1 + testdouble2; // benchmark
    add(op1_p,op2_p,op3_p);
    out(op1_p,op2_p,op3_p);
    }
    break;
    case 2: // odjemowanie
    {
    in(op1_p,op2_p,op3_p);
    if(op2.sign=='0')
        op2.sign='1';
    else
        op2.sign='0';
    testdouble3 = testdouble1 - testdouble2; // benchmark
    add(op1_p,op2_p,op3_p);
    out(op1_p,op2_p,op3_p);
    }
    break;
    case 3:
    {
    in(op1_p,op2_p,op3_p);
    mul(op1_p,op2_p,op3_p);
    out(op1_p,op2_p,op3_p);
    break;
    }
    default:
    printf("\nWybierz poprawna opcje :-)\n");
    break;
    case 6:
    {
        test();
    }
    break;
    }

    return 0;

}

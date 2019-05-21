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


xDouble* add(xDouble* op1, xDouble* op2, xDouble* op3,xDouble* op1_p,xDouble* op2_p,xDouble* op3_p)
{
    ///////////////////////////////////////////////////////
    // Dodawanie.
    // Dodawanie i odejmowanie mantys zaimplementowane jest w oddzielnych funkcjach
    ///////////////////////////////////////////////////////

    op3->sign = '0';
    op3->exponent = 0;
    op3->fraction1 = 0;
    op3->fraction2 = 0;

    if( (op1->sign != op2->sign) && (op1->exponent == op2->exponent) && (op1->fraction1 == op2->fraction1) && (op1->fraction2 == op2->fraction2)) // dodawanie liczb o przeciwnym znaku
    {
        // dwie liczby maja rowny modul i przeciwny znak - zerujemy
        op3->sign = '0';
        op3->exponent = 0;
        op3->fraction1 = 0;
        op3->fraction2 = 0;
        // i tu mozemy zakonczyc cala procedure
    }
    else // porownanie znakow zrobimy wtedy gdy zdecydujemy czy dodajemy czy odejmujemy
    {
        if(op1->exponent != op2->exponent)
        {
            int exp_diff = op1->exponent-op2->exponent; // Obliczanie roznicy wykladnikow.
            int exp_diff_abs; // Wartosc bezwzgledna.

            if(exp_diff < 0) exp_diff_abs = exp_diff * -1; // obliczanie wartości bezwzględnej
            else exp_diff_abs = exp_diff;

            // Przesuwamy w prawo (tracac precyzje) by wyrownac wykladniki.

            if(exp_diff > 0)
            {
                for(int i = 0; i < exp_diff_abs && i < 52; i++)
                {
                    op2->fraction2 >>= 1; // roundTowardZero - zaokraglenie przez obciecie
                    if (op2->fraction1 % 2 == 1)     // czy ostatni bit to 1?
                        op2->fraction2 += (1 << 31); // przenosimy na poczatek czesci 2
                    op2->fraction1 >>= 1;
                }
                op3->exponent = (op2->exponent = op1->exponent);

                op1->fraction1 += (1 << 20); // dodanie domyślnej jedynki op1.

                if(exp_diff_abs < 21)
                {
                    // Obie jedynki zostaną dodane w pierwszej czastce.
                    op2->fraction1 += (1 << (20 - (exp_diff_abs)));
                }
                else if (exp_diff_abs < 53)
                {
                    // Jedynka liczby op2 zostanie dodana w drugiej części.
                    op2->fraction2 += (1 << (52 - exp_diff_abs));
                    if (op2->fraction2 < (1 << (52 - exp_diff_abs)) )
                        op2->fraction1++; // nadmiar
                }

            }
            else//if(exp_diff < 0)
            {
                for(int i = 0; i < exp_diff_abs; i++)
                {
                    op1->fraction2 >>= 1; // roundTowardZero - zaokraglenie przez obciecie
                    if (op1->fraction1 % 2 == 1)     // czy ostatni bit to 1?
                        op1->fraction2 += (1 << 31); // przenosimy na poczatek czesci 2
                    op1->fraction1 >>= 1;
                }
                op3->exponent = (op1->exponent = op2->exponent);

                op2->fraction1 += (1 << 20); // dodanie domyślnej jedynki op1.

                if(exp_diff_abs < 21)
                {
                    // Obie jedynki zostaną dodane w pierwszej czastce.
                    op1->fraction1 += (1 <<  (20 - exp_diff_abs));
                }
                else
                {
                    // Jedynka liczby op2 zostanie dodana w drugiej części.
                    op1->fraction2 += (1 << (52 - exp_diff_abs));
                    if (op1->fraction2 < (1 << (52 - exp_diff_abs)) )
                        op1->fraction1++; // nadmiar
                }

            }


            if(op1->sign == op2->sign)
            {
                op3->sign = op1->sign;
                add_op1_op2_fraction(op1, op2, op3);
            }

            // a co jesli znaki są przeciwne?
            else if(op1->sign == '0' && exp_diff > 0)
            {
                op3->sign = '0';
                sub_op1_op2_fraction(op1, op2, op3);

            }
            else if(op1->sign == '0' && exp_diff < 0)
            {
                op3->sign = '1';
                sub_op2_op1_fraction(op1, op2, op3);
            }
            else if(op1->sign == '1' && exp_diff > 0)
            {
                op3->sign = '1';
                sub_op1_op2_fraction(op1, op2, op3);
            }
            else if(op1->sign == '1' && exp_diff < 0)
            {
                op3->sign = '0';
                sub_op2_op1_fraction(op1, op2, op3);
            }

        // Wyrownywanie potegi tak, by przed mantyse wystawala ostatnia, domyslna "1".

        int fract_dist = op3->fraction1>>20; // fraction distance - odległość

        if(fract_dist > 1)
        {
                do
                {
                    op3->fraction2 >>= 1; // roundTowardZero - zaokraglenie przez obciecie
                    if (op3->fraction1 % 2 == 1)     // czy ostatni bit to 1?
                        op3->fraction2 += (1 << 31); // przenosimy na poczatek czesci 2
                    op3->fraction1 >>= 1;
                    op3->exponent++;
                    fract_dist >>= 1;

                } while (fract_dist > 1);
        }
        else if(fract_dist < 1)
        {
                do
                {
                    op3->fraction1 <<= 1;
                    if (op3->fraction2 >> 31 == 1)     // czy ostatni bit to 1?
                        op3->fraction1 += 1; // przenosimy na poczatek czesci 2
                    op3->fraction2 <<= 1;
                    op3->exponent--;
                    fract_dist = op3->fraction1>>20;

                } while (fract_dist != 1);
        }

        }
        else // wykladniki sa rowne
        {
            op3->exponent = op1->exponent;

            if(op1->sign == '0' && op2->sign == '0')
            {
                op3->sign = '0';

                add_op1_op2_fraction(op1,op2,op3);

                if(op3->exponent != 0)
                {
                    op3->exponent+=1;
                    op3->fraction2>>=1;
                    if(op3->fraction1%2==1) op3->fraction2+=(1<<31);
                        op3->fraction1>>=1;
                }
                else
                {
                    if(op3->fraction1 >> 20 == 1)
                    {
                        op3->exponent+=1;
                    }
                }

            }
            else if(op1->sign == '1'&& op2->sign == '1')
            {
                op3->sign = '1';

                add_op1_op2_fraction(op1,op2,op3);

                op3->exponent+=1;
                op3->fraction2>>=1;
                if(op3->fraction1%2==1) op3->fraction2+=(1<<31);
                op3->fraction1>>=1;
            }
            else if(op1->sign != op2->sign)
            {
                if(op1->sign=='0')
                {
                    if(op1->fraction1>op2->fraction1)
                    {
                        op3->sign = '0';
                    }
                    else if(op1->fraction1 == op2->fraction1)
                    {
                        if(op1->fraction2>op2->fraction2)
                        {
                            op3->sign = '0';
                        }
                        else op3->sign = '1';
                    }
                    else
                    {
                        op3->sign = '1';
                    }

                    sub_op1_op2_fraction(op1,op2,op3);

                    if(op2->fraction1>op1->fraction1)
                    {
                        op3->fraction2=0-op3->fraction2;    // xor
                        if(op1->fraction2 < op2->fraction2) // niedomiar
                        {
                            op3->fraction1 -= 1; // przeniesienie do 1szej czesci mantysy
                        }
                        op3->fraction1=0-op3->fraction1;
                    }

                    int counter=0;
                    bool second = false;
                    for(counter=20;counter>0;counter--)
                    {
                        if((op3->fraction1>>counter)==1)
                        {
                            counter=20-counter;
                            break;
                        }
                    }
                    if(counter==0)
                    {
                    for(counter=32;counter>0;counter--)
                    {
                        if((op3->fraction2>>counter)==1)
                        {
                            second = true;
                            counter=32-counter+20;
                            break;
                        }
                    }
                    }
                    int parm=20;
                    if(second)
                    {
                        parm=52;
                    }
                    for(int shift=0; shift<counter && op3->exponent != 0; shift++)
                    {
                        op3->exponent-=1;
                        if(op3->exponent !=0)
                        {
                            op3->fraction1<<=1;
                            if(op3->fraction2>>31==1)
                            {
                                op3->fraction1+=1;
                            }
                            op3->fraction2<<=1;
                        }
                    }
                }
                else // if (op1->sign == '1')
                {
                     if(op1->fraction1>op2->fraction1)
                    {
                        op3->sign = '1';
                    }
                    else if(op1->fraction1 == op2->fraction1)
                    {
                        if(op1->fraction2>op2->fraction2)
                        {
                            op3->sign = '1';
                        }
                        else op3->sign = '0';
                    }
                    else
                    {
                        op3->sign = '0';
                    }

                    sub_op2_op1_fraction(op1,op2,op3);

                    if(op1->fraction1>op2->fraction1)
                    {
                        op3->fraction2=0-op3->fraction2;
                        if(op2->fraction2 < op1->fraction2) // niedomiar
                        {
                            op3->fraction1 -= 1; // przeniesienie do 1szej czesci mantysy
                        }
                        op3->fraction1=0-op3->fraction1;
                    }

                    int counter=0;
                    bool second = false;
                    for(counter=20;counter>0;counter--)
                    {
                        if((op3->fraction1>>counter)==1)
                        {
                            counter=20-counter;
                            break;
                        }
                    }
                    if(counter==0)
                    {
                    for(counter=32;counter>0;counter--)
                    {
                        if((op3->fraction2>>counter)==1)
                        {
                            second = true;
                            counter=32-counter+20;
                            break;
                        }
                    }
                    }
                    int parm=20;
                    if(second)
                    {
                        parm=52;
                    }
                    for(int shift=0;shift<counter;shift++)
                    {
                        op3->exponent-=1;
                        op3->fraction1<<=1;
                        if(op3->fraction2>>31==1)
                        {
                        op3->fraction1+=1;
                        }
                        op3->fraction2<<=1;
                    }
                }
            }
        }
    }
    op3->fraction1 = op3->fraction1 & ( (1 << 20) - 1 );  // wyczyszczenie pozostalych 1-ek przed mantysa

    return op3;
}

int menu()
{
int picked_option=0;
printf("\nCo byc chcial uczynic\n");
printf("1-Dodawanie\n");
printf("2-Odejmowanie\n");
printf("6-Test\n");

scanf("%d", &picked_option);
return picked_option;
}


int main()
{

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

    std::fesetround(FE_TONEAREST);

    switch(menu())
    {
    case 1: // dodawanie
    {
    in(op1_p,op2_p,op3_p);
    testdouble3 = testdouble1 + testdouble2; // benchmark
    std::fesetround(FE_TOWARDZERO);
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
    std::fesetround(FE_TOWARDZERO);
    add(op1_p,op2_p,op3_p);
    out(op1_p,op2_p,op3_p);
    }
    break;
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

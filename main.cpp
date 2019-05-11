#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include <mmintrin.h>
#include <immintrin.h>
#include <xmmintrin.h>
#include <emmintrin.h>

#include "operations.h"

#define FLT_RADIX 2

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

    double input;

    printf("Podaj pierwsza liczbe:");
    scanf("%lf",&input);

    union d64i_t
    {
        uint64_t integer_number;
        double double_number;
    } d64i; // unia sluzy do wydobycia bitow
    // reprezentacja 64-bitowa kopiowana jest bit po bicie a nastepnie odczytywana jako liczba calkowita
    d64i.double_number = input;
    uint64_t display_number = d64i.integer_number; // liczba jest "wyswietlana"
    // poprawnie nalezy teraz obliczac reszte z dzielenia przez 2 i zapisac do tabeli
    for(int i = 31; i >= 0; i--)
    {
        if (display_number % 2 == 0)
            op1.fraction2_s[i] = '0';
        else
        {
            op1.fraction2_s[i] = '1';
            op1.fraction2 += (1 << (31-i));
        }
        display_number >>= 1;
    }

    for(int i = 19; i >= 0; i--)
    {
        if (display_number % 2 == 0)
            op1.fraction1_s[i] = '0';
        else
        {
            op1.fraction1_s[i] = '1';
            op1.fraction1 += (1 << (19-i));
        }
        display_number >>= 1;
    }

    op1.exponent = 0;
    for(int i = 10; i >= 0; i--)
    {
        if (display_number % 2 == 0)
            op1.exponent_s[i] = '0';
        else
        {
            op1.exponent_s[i] = '1';
            op1.exponent += 1 << (10-i);
        }
        display_number >>= 1;
    }


    if (display_number % 2 == 0) op1.sign = '0';
    else op1.sign = '1';
    // drukuje na ekran pierwsza liczbew w formacie 1|11|52
    printf("%c|%s|%s %s\n",op1.sign,op1.exponent_s,op1.fraction1_s,op1.fraction2_s);

    printf("Podaj druga liczbe:");
    scanf("%lf",&input);

    d64i.double_number = input;
    display_number = d64i.integer_number;

    for(int i = 31; i >= 0; i--)
    {
        if (display_number % 2 == 0) op2.fraction2_s[i] = '0';
        else
        {
            op2.fraction2_s[i] = '1';
            op2.fraction2 += (1 << 31-i);
        }
        display_number >>= 1;
    }

    for(int i = 19; i >= 0; i--)
    {
        if (display_number % 2 == 0) op2.fraction1_s[i] = '0';
        else
        {
            op2.fraction1_s[i] = '1';
            op2.fraction1 += (1 << 19-i); // TODO 10 czy 19?
        }
        display_number >>= 1;
    }

    op2.exponent = 0;
    for(int i = 10; i >= 0; i--)
    {
        if (display_number % 2 == 0) op2.exponent_s[i] = '0';
        else
        {
            op2.exponent_s[i] = '1';
            op2.exponent += 1 << (10-i);
        }
        display_number >>= 1;
    }


    if (display_number % 2 == 0) op2.sign = '0';
    else op2.sign = '1';

    printf("%c|%s|%s %s\n",op2.sign,op2.exponent_s,op2.fraction1_s, op2.fraction2_s);

    ///////////////////////////////////////////////////////
    // Dodawanie po normalności. Bez użycia akcelerancji.//
    // Należy to potem zankcelerownać.                   //
    ///////////////////////////////////////////////////////

    op3.sign = '0';
    op3.exponent = 0;
    op3.fraction1 = 0;
    op3.fraction2 = 0;

    if( (op1.sign != op2.sign) && (op1.exponent == op2.exponent) && (op1.fraction1 == op2.fraction1) && (op1.fraction2 == op2.fraction2)) // dodawanie liczb o przeciwnym znaku
    {
        // dwie liczby maja rowny modul i przeciwny znak - zerujemy
        op3.sign = '0';
        op3.exponent = 0;
        op3.fraction1 = 0;
        op3.fraction2 = 0;
        // i tu mozemy zakonczyc cala procedure
    }
    else // porownanie znakow zrobimy wtedy gdy zdecydujemy czy dodajemy czy odejmujemy
    {
        if(op1.exponent != op2.exponent)
        {
            int exp_diff = op1.exponent-op2.exponent; // Obliczanie roznicy wykladnikow.
            int exp_diff_abs; // Wartosc bezwzgledna.

            if(exp_diff < 0) exp_diff_abs = exp_diff * -1; // obliczanie wartości bezwzględnej
            else exp_diff_abs = exp_diff;

            // Przesuwamy w prawo (tracac precyzje) by wyrownac wykladniki.

            if(exp_diff > 0)
            {
                for(int i = 0; i < exp_diff_abs; i++)
                {
                    op2.fraction2 >>= 1; // roundTowardZero - zaokraglenie przez obciecie
                    if (op2.fraction1 % 2 == 1)     // czy ostatni bit to 1?
                        op2.fraction2 += (1 << 31); // przenosimy na poczatek czesci 2
                    op2.fraction1 >>= 1;
                }
            }
            else//if(exp_diff < 0)
            {
                for(int i = 0; i < exp_diff_abs; i++)
                {
                    op1.fraction2 >>= 1; // roundTowardZero - zaokraglenie przez obciecie
                    if (op1.fraction1 % 2 == 1)     // czy ostatni bit to 1?
                        op1.fraction2 += (1 << 31); // przenosimy na poczatek czesci 2
                    op1.fraction1 >>= 1;
                }
            }
            op3.exponent = (op1.exponent = op2.exponent);

            op3.fraction1 += (1 << 20); // dodanie domyślnej jedynki op1.

            if(exp_diff_abs < 20)
            {
                // Obie jedynki zostaną dodane w pierwszej czastce.
                op3.fraction1 += (1 << (20 - exp_diff_abs));
            }
            else
            {
                // Jedynka liczby op2 zostanie dodana w drugiej części.
                op3.fraction2 += (1 << (52 - exp_diff_abs));
            }

            if(op1.sign == op2.sign)
            {
                op3.sign = op1.sign;
                add_op1_op2_fraction(op1_p, op2_p, op3_p);
            }


            // a co jesli znaki są przeciwne?
            else if(op1.sign == '0' && exp_diff > 0)
            {
                op3.sign = '0';
                sub_op1_op2_fraction(op1_p, op2_p, op3_p);

            }
            else if(op1.sign == '0' && exp_diff < 0)
            {
                op3.sign = '1';
                sub_op1_op2_fraction(op1_p, op2_p, op3_p);
            }
            else if(op1.sign == '1' && exp_diff > 0)
            {
                op3.sign = '1';
                sub_op1_op2_fraction(op1_p, op2_p, op3_p);
            }
            else if(op1.sign == '1' && exp_diff < 0)
            {
                op3.sign = '0';
                sub_op1_op2_fraction(op1_p, op2_p, op3_p);
            }

        // Wyrownywanie potegi tak, by przed mantyse wystawala ostatnia, domyslna "1".

        int fract_dist = op3.fraction1>>20;
        if(fract_dist > 1)
        {
                do
                {
                    op3.fraction2 >>= 1; // roundTowardZero - zaokraglenie przez obciecie
                    if (op3.fraction1 % 2 == 1)     // czy ostatni bit to 1?
                        op3.fraction2 += (1 << 31); // przenosimy na poczatek czesci 2
                    op3.fraction1 >>= 1;
                    op3.exponent++;
                    fract_dist >>= 1;

                } while (fract_dist > 1);
        }

        op3.fraction1 = op3.fraction1 & ( (1 << 20) - 1 );  // wyczyszczenie pozostalych 1-ek przed mantysa

        }
        else // wykladniki sa rowne
        {
            op3.exponent = op1.exponent;

            if(op1.sign == '0' && op2.sign == '0')
            {
                op3.sign = '0';
                add_op1_op2_fraction(op1_p, op2_p, op3_p);

                if(op3.fraction1 > 1048575) // nadmiar
                {
                    op3.exponent+=op3.fraction1/(1<<20); // przeniesienie do 1szej czesci mantysy
                    if(op3.fraction1%2==1)
                    {
                        op3.fraction2>>=1;
                        op3.fraction2+=(1<<31);
                    }
                    op3.fraction1>>= 1;

                }
                if(op1.fraction2 == op2.fraction2)
                {
                    op3.sign = '0';
                    op3.fraction1=op2.fraction1;
                    op3.fraction2=op2.fraction2;
                    op3.exponent=op2.exponent+1;// przepełnienie tutaj wchodzi na specjalne liczby
                }
            }
            else if(op1.sign == '1'&& op2.sign == '1')
            {
               op3.sign = '1';


                add_op1_op2_fraction(op1_p, op2_p, op3_p);

                if(op3.fraction1 > 1048575) // nadmiar
                {
                    op3.exponent+=op3.fraction1/(1<<20); // przeniesienie do 1szej czesci mantysy
                    if(op3.fraction1%2==1)
                    {
                        op3.fraction2>>=1;
                        op3.fraction2+=(1<<31);
                    }
                    op3.fraction1>>= 1;

                }
                if(op1.fraction2 == op2.fraction2)
                {
                    op3.sign = '0';
                    op3.fraction1=op2.fraction1;
                    op3.fraction2=op2.fraction2;
                    op3.exponent=op2.exponent+1;// przepełnienie tutaj wchodzi na specjalne liczby
                }
            }
            else if(op1.sign != op2.sign)
            {

            }
        }

    }




    fflush(stdout);
/////////////// DRUkuj w postaci binarnej.

    display_number = op3.fraction2;
    for(int i = 31; i >= 0; i--)
    {
        if (display_number % 2 == 0) op3.fraction2_s[i] = '0';
        else op3.fraction2_s[i] = '1';
        display_number >>= 1;
    }

    display_number = op3.fraction1;
    for(int i = 19; i >= 0; i--)
    {
        if (display_number % 2 == 0) op3.fraction1_s[i] = '0';
        else op3.fraction1_s[i] = '1';
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


    display_number = op3.exponent;
    for(int i = 10; i >= 0; i--)
    {
        if (display_number % 2 == 0) op3.exponent_s[i] = '0';
        else op3.exponent_s[i] = '1';
        display_number >>= 1;
    }

    printf("Suma:\n");
    printf("%c|%s|(%s)%s %s\n",op3.sign,op3.exponent_s,fraction0_s, op3.fraction1_s,op3.fraction2_s);

    /*
        ////////////////////////////////////////////
        // Konwersja do rejestrów i dodawanie mantys.

        union{ __m64 mantissas; uint32_t addends[2];} ma1, ma2, ma3; // 12|20|32|12|20|32 bity


        ma1.addends[0] = 0;
        for(uint32_t i = 0; i < 20; i++){
            if(op1.fraction1_s[i] == '1')
                ma1.addends[0] += (1 << (20-i));
        }

        ma1.addends[1] = 0;
        for(uint32_t i = 0; i < 32; i++){
            if(op1.fraction2_s[i] == '1')
                ma1.addends[1] += (1 << (32-i));
        }
        ma2.addends[0] = 0;
        for(uint32_t i = 0; i < 20; i++){
            if(op2.fraction1_s[i] == '1')
                ma2.addends[0] += (1 << (20-i));
        }

        ma2.addends[1] = 0;
        for(uint32_t i = 0; i < 32; i++){
            if(op2.fraction2_s[i] == '1')
                ma2.addends[1] += (1 << (32-i));
        }

        printf("%u %u %u %u\n", ma1.addends[0], ma1.addends[1], ma2.addends[0], ma2.addends[1]);

        __m64 a, b, c;

        a = ma1.mantissas;
        b = ma2.mantissas;

        if(op1.exponent < op2.exponent)
        {
            op3.exponent = op2.exponent;
            op3.sign = op2.sign;
            int diff = op2.exponent - op1.exponent;
            a = _m_psrlqi(a,diff);    }
        else
        {
            op3.exponent = op1.exponent;
            op3.sign = op1.sign;
            int diff = op1.exponent - op2.exponent;
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
        op3.exponent++;
        c = _m_psrlqi(c,1); // przesuniecie w prawo o 1 pozycje, wypelnienie zerami
        }

        ma3.mantissas = c;

        for (int i = 0; i < 32; i++)
        {
            if( decode_number % 2 == 0) op3.fraction2_s[31-i] = '0';
            else op3.fraction2_s[31-i] = '1';
            decode_number >>= 1;
        }

        decode_number = ma3.addends[0];
        for (int i = 0; i < 20; i++)
        {
            if( decode_number % 2 == 0) op3.fraction1_s[19-i] = '0';
            else op3.fraction1_s[19-i] = '1';
            decode_number >>= 1;
        }

        decode_number = op3.exponent;
        for (int i = 0; i < 12; i++)
        {
            if( decode_number % 2 == 0) op3.exponent_s[10-i] = '0';
            else op3.exponent_s[10-i] = '1';
            decode_number >>= 1;
        }
    */
//    printf("%c|%s|%s %s\n",op3.sign,op3.exponent_s,op3.fraction1_s,op3.fraction2_s);

    return 0;

}

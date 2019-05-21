#include <mmintrin.h>
#include <stdio.h>
#include <string.h>
#include "operations.h"

void add_op1_op2_fraction(xDouble* op1, xDouble* op2, xDouble* op3)
{

    __m64 reg1 = _mm_set_pi32 (op1->fraction1, op1->fraction2); //op1[fraction2, fraction1]
    __m64 reg2 = _mm_set_pi32 (op2->fraction1, op2->fraction2);
    __m64 reg3 = _mm_add_pi32(reg1, reg2);

    if((uint32_t) reg3[0] < (uint32_t) reg1[0] || (uint32_t) reg3[0] < (uint32_t) reg2[0]) // nadmiar
    {
        reg3[1] +=1; // przeniesienie do 1szej czesci mantysy
    }

    op3->fraction2 = reg3[0];
    op3->fraction1 = reg3[1];

    _mm_empty();
}


void sub_op1_op2_fraction(xDouble* op1, xDouble* op2, xDouble* op3)
{
    __m64 reg1 = _mm_set_pi32 (op1->fraction1, op1->fraction2);
    __m64 reg2 = _mm_set_pi32 (op2->fraction1, op2->fraction2);
    __m64 reg3 = _mm_sub_pi32(reg1, reg2);

    if((uint32_t) reg2[0] > (uint32_t) reg1[0]) // nadmiar
    {
        reg3[1] -=1; // przeniesienie do 1szej czesci mantysy
    }

    op3->fraction2 = reg3[0];
    op3->fraction1 = reg3[1];

    _mm_empty();
}

void sub_op2_op1_fraction(xDouble* op1, xDouble* op2, xDouble* op3)
{
    __m64 reg1 = _mm_set_pi32 (op1->fraction1, op1->fraction2);
    __m64 reg2 = _mm_set_pi32 (op2->fraction1, op2->fraction2);
    __m64 reg3 = _mm_sub_pi32(reg2, reg1);

    if( (uint32_t) reg1[0] > (uint32_t) reg2[0]) // nadmiar
    {
        reg3[1] -=1; // przeniesienie do 1szej czesci mantysy
    }

    op3->fraction2 = reg3[0];
    op3->fraction1 = reg3[1];

}

void test()
{
    double testdouble1, testdouble2, testdouble3, testdouble4;

    int correct = 0;
    int incorrect = 0;

    xDouble op1;
    xDouble op2;
    xDouble op3;
    xDouble* op1_p = &op1;
    xDouble* op2_p = &op2;
    xDouble* op3_p = &op3;

    xDouble* sum;

    char operation;
    FILE *datafile = fopen("data.txt", "r");

    do{
        //fscanf(datafile, " %c %lf %lf", &operation, &testdouble1, &testdouble2);
        op1.exponent = 0;
        op1.fraction1 = 0;
        op1.fraction2 = 0;

        op2.exponent = 0;
        op2.fraction1 = 0;
        op2.fraction2 = 0;

        op3.exponent = 0;
        op3.fraction1 = 0;
        op3.fraction2 = 0;

        char input_string[256];
        fgets(input_string, 256, datafile);
        operation = input_string[0];
        if (operation != 'A' && operation != 'S') continue;
        sscanf(input_string, " %c %lf %lf ", &operation, &testdouble1, &testdouble2);

        double input = testdouble1;

        d64i.double_number = input;
        uint64_t display_number = d64i.integer_number;

        // liczba jest "wyswietlana"
        // poprawnie nalezy teraz obliczac reszte z dzielenia przez 2 i zapisac do tabeli
        for(int i = 31; i >= 0; i--)
        {
            if (display_number % 2 == 1) op1.fraction2 += (1 << (31-i));
            display_number >>= 1;
        }

        for(int i = 19; i >= 0; i--)
        {
            if (display_number % 2 == 1) op1.fraction1 += (1 << (19-i));
            display_number >>= 1;
        }

        for(int i = 10; i >= 0; i--)
        {
            if (display_number % 2 == 1) op1.exponent += 1 << (10-i);
            display_number >>= 1;
        }


        if (display_number % 2 == 0) op1.sign = '0';
        else op1.sign = '1';
        // wczytywanie drugiej liczby
        input = testdouble2;

        d64i.double_number = input;
        display_number = d64i.integer_number;

        for(int i = 31; i >= 0; i--)
        {
            if (display_number % 2 == 1) op2.fraction2 += (1 << (31-i));
            display_number >>= 1;
        }

        for(int i = 19; i >= 0; i--)
        {
            if (display_number % 2 == 1) op2.fraction1 += (1 << (19-i));
            display_number >>= 1;
        }

        for(int i = 10; i >= 0; i--)
        {
            if (display_number % 2 == 1) op2.exponent += 1 << (10-i);
            display_number >>= 1;
        }

        if (display_number % 2 == 0) op2.sign = '0';
        else op2.sign = '1';

        if(operation == 'A')
        {
            testdouble3 = testdouble1 + testdouble2;
        }
        else if(operation == 'S')
        {
            testdouble3 = testdouble1 - testdouble2;
            if(op2.sign == '0') op2.sign = '1';
            else op2.sign = '0';
        }
        else continue;


        sum = add(op1_p, op2_p, op3_p);
        d64i.integer_number = 0;
        if (sum->sign == '1')
            d64i.integer_number += 1 << 11;

        d64i.integer_number += sum->exponent;
        d64i.integer_number <<= 20;
        d64i.integer_number += sum->fraction1;
        d64i.integer_number <<= 32;
        d64i.integer_number += sum->fraction2;

        testdouble4 = (double) d64i.double_number;

        printf("%.20e", testdouble1);

        if(operation == 'A')
            printf(" + ");
        else if (operation == 'S')
            printf(" - ");

        printf("%.20e = %.20e <=> %.20e ? ", testdouble2, testdouble3, d64i.double_number);

        if (testdouble3 == d64i.double_number)
        {
            printf("OK\n");
            correct++;
        }
        else
        {
            printf("NO\n");
            incorrect++;
        }
    }
    while(feof(datafile) == 0);

    printf("Poprawnych: %d; niepoprawnych: %d\n", correct, incorrect);

    fclose(datafile);
}

xDouble* add(xDouble* op1, xDouble* op2, xDouble* op3)
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
                for(int i = 0; i < exp_diff_abs  && i < 52; i++)
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
                else if (exp_diff_abs < 53)
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


                    if(op3->exponent == 0 && op2->fraction1 > op1->fraction1 || op2->fraction2 > op1->fraction2)
                    {
                        sub_op2_op1_fraction(op1,op2,op3);
                    }
                    else
                        sub_op1_op2_fraction(op1,op2,op3);

                    if(op2->fraction1>op1->fraction1)
                    {
                            op3->fraction2=0-op3->fraction2;    // NOT / XOR 0
                            if(op1->fraction2 < op2->fraction2) // niedomiar
                            {
                                op3->fraction1 += 1; // przeniesienie do 1szej czesci mantysy
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
                            op3->fraction1 += 1; // przeniesienie do 1szej czesci mantysy
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


#include <mmintrin.h>
#include <stdio.h>
#include "operations.h"

void add_op1_op2_fraction(xDouble* op1, xDouble* op2, xDouble* op3)
{

    __m64 reg1 = _mm_set_pi32 (op1->fraction1, op1->fraction2);
    __m64 reg2 = _mm_set_pi32 (op2->fraction1, op2->fraction2);
    __m64 reg3 = _mm_add_pi32(reg1, reg2);

    if(reg3[0] < reg1[0] || reg3[0] < reg2[0]) // nadmiar
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

    if(reg2[0] > reg1[0]) // nadmiar
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

    if(reg1[0] > reg2[0]) // nadmiar
    {
        reg3[1] -=1; // przeniesienie do 1szej czesci mantysy
    }

    op3->fraction2 = reg3[0];
    op3->fraction1 = reg3[1];

}

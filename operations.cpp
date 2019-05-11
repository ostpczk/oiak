#include "operations.h"

void add_op1_op2_fraction(xDouble* op1, xDouble* op2, xDouble* op3)
{
    op3->fraction2 += op1->fraction2; // tu mozna zastosowac
    op3->fraction2 += op2->fraction2; // akcelerancje sprzentowom
    if(op3->fraction2 < op1->fraction2 || op3->fraction2 < op2->fraction2) // nadmiar
    {
        op3->fraction1 +=1; // przeniesienie do 1szej czesci mantysy
    }
    op3->fraction1 += op1->fraction1;
    op3->fraction1 += op2->fraction1;
}


void sub_op1_op2_fraction(xDouble* op1, xDouble* op2, xDouble* op3)
{
    op3->fraction2 += op1->fraction2; // tu mozna zastosowac
    op3->fraction2 -= op2->fraction2; // akcelerancje sprzentowom

    op3->fraction1 += op1->fraction1;
    op3->fraction1 -= op2->fraction1;

    if(op2->fraction2 > op1->fraction2) // nadmiar
    {
        op3->fraction1 -= 1; // przeniesienie do 1szej czesci mantysy
    }
}

void sub_op2_op1_fraction(xDouble* op1, xDouble* op2, xDouble* op3)
{
    op3->fraction2 += op2->fraction2; // tu mozna zastosowac
    op3->fraction2 -= op1->fraction2; // akcelerancje sprzentowom
    if(op1->fraction2 > op2->fraction2) // nadmiar
    {
        op3->fraction1 -= 1; // przeniesienie do 1szej czesci mantysy
    }
    op3->fraction1 += op2->fraction1;
    op3->fraction1 -= op1->fraction1;
}

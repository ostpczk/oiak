#ifndef OPERATIONS_H
#define OPERATIONS_H
#include <stdint.h>

extern double testdouble1;
extern double testdouble2;
extern double testdouble3;

extern union d64i_t
    {
        uint64_t integer_number;
        double double_number;
    } d64i; // unia sluzy do wydobycia bitow
    // reprezentacja 64-bitowa kopiowana jest bit po bicie a nastepnie odczytywana jako liczba calkowita

typedef struct xDouble
{
        char sign;

        char exponent_s[12];

        uint16_t exponent;

        char fraction1_s[21]; // mantysa podzielona na dwie części
        char fraction2_s[33]; // 20 + 32 bity

        uint32_t fraction1;
        uint32_t fraction2;

};

void add_op1_op2_fraction(xDouble* op1, xDouble* op2, xDouble* op3);
void sub_op1_op2_fraction(xDouble* op1, xDouble* op2, xDouble* op3);
void sub_op2_op1_fraction(xDouble* op1, xDouble* op2, xDouble* op3);

void add_op1_op2_fraction_with(xDouble* op1, xDouble* op2, xDouble* op3);
void sub_op1_op2_fraction_with(xDouble* op1, xDouble* op2, xDouble* op3);
void sub_op2_op1_fraction_with(xDouble* op1, xDouble* op2, xDouble* op3);

void test();

xDouble* add(xDouble* op1, xDouble* op2, xDouble* op3);
xDouble* mul(xDouble* op1, xDouble* op2, xDouble* op3);


#endif // OPERATIONS_H

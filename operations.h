#ifndef OPERATIONS_H
#define OPERATIONS_H
#include <stdint.h>

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



#endif // OPERATIONS_H

#ifndef XDOUBLE_H
#define XDOUBLE_H
#include <stdint.h>



class xDouble
{
    public:
        xDouble();
        virtual ~xDouble();
        char sign;

        char exponent[12];

        uint16_t exponent_int;

        char fraction1[21]; // mantysa podzielona na dwie części
        char fraction2[33]; // 20 + 32 bity

        uint32_t fraction1_int;
        uint32_t fraction2_int;

    protected:

    private:
};

#endif // XDOUBLE_H

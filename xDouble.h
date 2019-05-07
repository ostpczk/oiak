#ifndef XDOUBLE_H
#define XDOUBLE_H
#include <stdint.h>



class xDouble
{
    public:
        xDouble();
        virtual ~xDouble();
        char sign;

        char exponent_s[12];

        uint16_t exponent;

        char fraction1_s[21]; // mantysa podzielona na dwie części
        char fraction2_s[33]; // 20 + 32 bity

        uint32_t fraction1;
        uint32_t fraction2;

    protected:

    private:
};

#endif // XDOUBLE_H

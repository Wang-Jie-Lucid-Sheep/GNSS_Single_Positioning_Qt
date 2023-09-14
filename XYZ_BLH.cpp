#include "Function.h"

void XYZ_BLH(double X, double Y, double Z, double* B, double* L, double* h)
{
    // Access the matrix using the pointer
    double a84 = 6378137.0;
    double b84 = 6356752.31;
    double ee ;
    ee = (pow(a84, 2) - pow(b84, 2)) / pow(a84, 2);
    double p = sqrt(pow(X, 2) + pow(Y, 2));
    double B0 = atan2(Z, p);
    double N0 = a84 / sqrt(1 - ee * pow(sin(B0), 2));
    (*B) = atan2(Z + N0 * ee * sin(B0), p);
    while (((*B) - B0) > 0.001)
    {
        B0 = (*B);
        N0 = a84 / sqrt(1 - ee * pow(sin(B0), 2));
        (*B) = atan2(Z + N0 * ee * sin(B0), p);
    }
    *L = atan2(Y, X);
    *h = p / cos(*B) - N0;

}

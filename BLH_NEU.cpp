#include "Function.h"

void BLH_NEU(double B, double L, double X, double Y, double Z, double* N, double* E, double* U)
{
	MatrixXd S(3, 3);
	MatrixXd e(3, 1);
	MatrixXd x(3, 1);
	S(0, 0) = -sin(L);
	S(0, 1) = cos(L);
	S(0, 2) = 0;
	S(1, 0) = -sin(B) * cos(L);
	S(1, 1) = -sin(B) * sin(L);
	S(1, 2) = cos(B);
	S(2, 0) = cos(B) * cos(L);
	S(2, 1) = cos(B) * sin(L);
	S(2, 2) = sin(B);
	x(0, 0) = X;
	x(1, 0) = Y;
	x(2, 0) = Z;
	e = S * x;
	*E = e(0, 0);
	*N = e(1, 0);
	*U = e(2, 0);
}
#include "Function.h"

double YMD_JulianDay(std::string YMD_Time)
{
	double Y;
	double M;
	double D;
	double Hour;
	Y = std::stod((YMD_Time.substr(2, 2)));
	M = std::stod((YMD_Time.substr(4, 2)));
	D = std::stod((YMD_Time.substr(6, 2)));
	Hour= std::stod((YMD_Time.substr(8, 2)))+ std::stod((YMD_Time.substr(10, 2)))/60+ std::stod((YMD_Time.substr(12, 2)))/3600;
	if (Y < 80)
	{
		Y += 2000;
	}
	else
	{
		Y += 1900;
	}
	if ( M <= 2)
	{
		Y -= 1;
		M = +12;
	}
	double JulianDay =floor(365.25*Y)+floor(30.6001*(M+1))+D+Hour/24+1720981.5;
	return JulianDay;
}
double JulianDay_GPST(double JulianDay)
{
	double GPST = fmod(JulianDay - 2444244.5, 7) * 86400;
	return  GPST;
}
#include "Function.h"
double Trop_Delay(double ele, double lat, double h)
{
	double humi = 0;
	double hgt = 0;
	if (h < 0)
		hgt = 0;
	else
		hgt = h;
	double pres = pow(1013.25 * (1 - 2.2557e-5 * hgt), 5.2568);
	double temp = 15 - 6.5e-3 * hgt + 273.16;
	double e = 6.108 * humi * exp((17.15*temp-4684)/(temp-38.45));
	double z = PI / 2 - ele;
	double trph = 0.0022768 * pres / (1 - 0.00266 * cos(2 * lat) - 0.00028 * hgt / 1e3) / cos(z);
	double trpw = 0.002277 * (1255 / temp + 0.05) * e / cos(z);
	double trop_delay = trph + trpw;
	return trop_delay;
}
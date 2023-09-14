#include "Function.h"
//#include "File_Read.h"
using namespace std;
void Satellite_Position(double tt, int best_satellite_site, double mts, double* delta_t, double* Xk, double* Yk, double* Zk,double* Data,string PRN,double S_GPST)
{
	string GEO[11] = {"02","03","04","06","11","16","23","41","45","54","55"};
	if (PRN == "G")
	{
		double u = 3.986004418e+14;//万有引力常数
		double we = 7.29211567e-5;//地球自转角速度
		//
		double n0 = sqrt(u) / pow(Data S_Sqrt_A, 3);
		double n = n0 + Data S_delta_n;
		//
		*delta_t = Data S_a0 + Data S_a1 * (tt - S_GPST) + Data S_a2 * pow(tt - S_GPST, 2);
		double t = tt - *delta_t;
		double tk = t - Data S_TOE;
		if (tk>302400)
		{
			tk -= 604800;
		}
		else if (tk<-302400)
		{
			tk += 604800;
		}
		//
		double Mk = Data S_M0 + n * tk;
		//
		double Ek = Mk + (Data S_e - 1 / 8 * pow(Data S_e, 3) + 1 / 192 * pow(Data S_e, 5) - 1 / 9216 * pow(Data S_e, 7)) * sin(Mk)
			+ (1 / 2 * pow(Data S_e, 2) - 1 / 6 * pow(Data S_e, 4) + 1 / 98 * pow(Data S_e, 6)) * sin(2 * Mk)
			+ (3 / 8 * pow(Data S_e, 3) - 27 / 128 * pow(Data S_e, 5) + 243 / 5120 * pow(Data S_e, 7)) * sin(3 * Mk)
			+ (1 / 3 * pow(Data S_e, 4) - 4 / 15 * pow(Data S_e, 6)) * sin(4 * Mk)
			+ (125 / 384 * pow(Data S_e, 5) - 3125 / 9216 * pow(Data S_e, 7)) * sin(5 * Mk)
			+ 27 / 80 * pow(Data S_e, 6) * sin(6 * Mk) + 16807 / 46080 * pow(Data S_e, 7) * sin(7 * Mk);
		//
		double cos_Vk = (cos(Ek) - Data S_e) / (1 - Data S_e * cos(Ek));
		double sin_Vk = (sqrt(1 - pow(Data S_e, 2)) * sin(Ek)) / (1 - Data S_e * cos(Ek));
		double Vk = 0;
		if (cos_Vk==0)
		{
			if (sin_Vk > 0)
			{
				Vk = PI / 2;
			}
			else
			{
				Vk = -PI / 2;
			}
		}
		else
		{
			Vk = atan(sqrt(1 - pow(Data S_e, 2)) * sin(Ek) / (cos(Ek) - Data S_e));
		}
		if (cos_Vk < 0)
		{
			if (sin_Vk >= 0)
			{
				Vk = PI + atan(sqrt(1 - pow(Data S_e, 2)) * sin(Ek) / (cos(Ek) - Data S_e));
			}
			else
			{
				Vk = atan(sqrt(1 - pow(Data S_e, 2)) * sin(Ek) / (cos(Ek) - Data S_e)) - PI;
			}
		}
		//
		double Fk = Vk + Data S_omega;
		//
		double Delta_u = Data S_Cuc * cos(2 * Fk) + Data S_Cus * sin(2 * Fk);
		double Delta_r = Data S_Crc * cos(2 * Fk) + Data S_Crs * sin(2 * Fk);
		double Delta_i = Data S_Cic * cos(2 * Fk) + Data S_Cis * sin(2 * Fk);
		//
		double uk = Fk + Delta_u;
		double rk = pow(Data S_Sqrt_A, 2) * (1 - Data S_e * cos(Ek)) + Delta_r;
		double ik = Data S_i0 + Delta_i + Data S_IDOT * tk;
		//
		double xk = rk * cos(uk);
		double yk = rk * sin(uk);
		//
		double Omega_k = Data S_OMEGA + (Data S_OMEGA_DOT - we) * tk - we * Data S_TOE;
		//
		MatrixXd S_POS(3, 1);
		double S_X = xk * cos(Omega_k) - yk * cos(ik) * sin(Omega_k);
		double S_Y = xk * sin(Omega_k) + yk * cos(ik) * cos(Omega_k);
		double S_Z = yk * sin(ik);

		S_POS(0, 0) = S_X;//xk * cos(Omega_k) - yk * cos(ik) * sin(Omega_k);
		S_POS(1, 0) = S_Y;//xk * sin(Omega_k) + yk * cos(ik) * cos(Omega_k);
		S_POS(2, 0) = S_Z;//yk * sin(ik);
		//
		double aph = we * mts;
		MatrixXd T(3, 3);
		MatrixXd temp_xyz(3, 1);
		/*
		T << 0, sin(aph), 0
			- sin(aph), 0, 0,
			0, 0, 0;
		*/
		T(0, 0) = 0;
		T(0, 1) = sin(aph);
		T(0, 2) = 0;
		T(1, 0) = -sin(aph);
		T(1, 1) = 0;
		T(1, 2) = 0;
		T(2, 0) = 0;
		T(2, 1) = 0;
		T(2, 2) = 0;
		temp_xyz = T * S_POS;
		*Xk = temp_xyz(0, 0) + S_X;
		*Yk = temp_xyz(1, 0) + S_Y;
		*Zk = temp_xyz(2, 0) + S_Z;
		//
		double real = -2 * sqrt(u) * Data S_e * Data S_Sqrt_A * sin(Ek) / pow(2.997925e+8, 2);
		*delta_t = real + *delta_t;
	}
	else if (PRN == "C")
	{

	}


}

double Satellite_ele(double N, double E, double U)
{
	double S_ele = 0;
	S_ele = asin(U / sqrt(pow(E, 2) + pow(N, 2) + pow(U, 2)));
	return S_ele;
}
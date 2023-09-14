#pragma once
#ifndef FUNCYION_H
#define FUNCYION_H
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <string.h>
#include <algorithm>
#include <cmath>
#include <Eigen/Dense>
//#include "File_Read.h"
#define PI acos(-1)

#define S_a0 [0]
#define S_a1 [1]
#define S_a2 [2]
#define S_IODE [3]
#define S_Crs [4]
#define S_delta_n [5]
#define S_M0 [6]
#define S_Cuc [7]
#define S_e [8]
#define S_Cus [9]
#define S_Sqrt_A [10]
#define S_TOE [11]
#define S_Cic [12]
#define S_OMEGA [13]
#define S_Cis [14]
#define S_i0 [15]
#define S_Crc [16]
#define S_omega [17]
#define S_OMEGA_DOT [18]
#define S_IDOT [19]
#define S_L2ma [20]
#define S_GPS_circle [21]
#define S_L2_P_ma [22]
#define S_Accuracy [23]
#define S_Health [24]
#define S_TGD [25]
#define S_IODC_clock [26]
#define S_HOW [27]
#define S_h [28]

using namespace Eigen;

// Declare the function in the header file
void XYZ_BLH(double X, double Y, double Z,double *B,double *L,double *h);
void BLH_NEU(double B, double L, double X, double Y, double Z, double* N, double* E, double* U);
double YMD_JulianDay(std::string YMD_Time);
double JulianDay_GPST(double JulianDay);
int Find_best_satellite(int middle, MatrixXd *comp_mat);
void Satellite_Position(double tt, int best_satellite_site, double mts, double* delta_t, double* Xk, double* Yk, double* Zk,double* Data,std::string PRN,double S_GPST);
double Satellite_ele(double N, double E, double U);
double Trop_Delay(double ele,double lat,double h);

#endif

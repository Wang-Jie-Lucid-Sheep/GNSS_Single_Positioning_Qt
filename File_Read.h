#pragma once
#ifndef FILE_READ_H
#define FILE_READ_H
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <string.h>
#include <algorithm>


std::vector<std::vector<std::string>> SYS_OBS_TYPES;

struct struct_line {
    std::string sName;
    std::vector<double> vValues;

    double get_value(std::string s) {
        std::string sTemp;
        for (int i = 0; i < SYS_OBS_TYPES.size(); i++) {
            if (SYS_OBS_TYPES[i][0][0] == sName[0]) {
                std::vector<std::string>::iterator it = std::find(SYS_OBS_TYPES[i].begin(), SYS_OBS_TYPES[i].end(), s);
                if (it != SYS_OBS_TYPES[i].end()) {
                    return vValues[it - SYS_OBS_TYPES[i].begin() - 2];
                }
                else
                    return -1;
            }
        }
    }
};

struct struct_o {
    std::string sTime;
    int year, month, day, hour, minute, second;
    bool is_error;
    int nValue_num;
    std::vector<struct_line> vData_o;
};

struct struct_p {
    std::string sTime;
    std::vector<struct_line> vData_p;
};

class Cdata {
protected:
    double year, month, day, hour, minute, second;
    double dAPPROX_POSITION[3];
    double dDELTA[3];

    std::vector<struct_o> data_o;
    std::vector<struct_p> data_p;

public:

    void read_o(std::string path);
    void read_p(std::string path);
    int get_o_size();
    int get_p_size();
    double get_year() { return year; }
    double get_month() { return month; }
    double get_day() { return day; }
    double get_hour() { return hour; }
    double get_minute() { return minute; }
    double get_second() { return second; }
    double get_dAPPROX_POSITION(int num) { return *(dAPPROX_POSITION + num); }
    double get_dDELTA(int num) { return *(dDELTA + num); }
    double get_obs_num(char s) {
        std::string sTemp;
        for (int i = 0; i < SYS_OBS_TYPES.size(); i++) {
            if (SYS_OBS_TYPES[i][0][0] == s) {
                sTemp = SYS_OBS_TYPES[i][1];
                return stod(sTemp);
            }
        }
    }

    struct_line Getline_Value_o(std::string sline);
    struct_o get_sData_o(int num) { return data_o[num]; }
    struct_p get_sData_p(int num) { return data_p[num]; }
};

int Cdata::get_o_size()
{
    return data_o.size();
}

int Cdata::get_p_size()
{
    return data_p.size();
}

struct_line Cdata::Getline_Value_o(std::string sline) {
    int nNum = 0;
    struct_line sLine;
    std::string temp;
    for (int i = 0; i < 3; i++)
        sLine.sName += sline[i];
    nNum = get_obs_num(sLine.sName[0]);
    if (sLine.sName[0] == 'G')
        nNum = 12;
    else if (sLine.sName[0] == 'R')
        nNum = 9;
    else if (sLine.sName[0] == 'C')
        nNum = 6;
    else if (sLine.sName[0] == 'E')
        nNum = 12;
    else if (sLine.sName[0] == 'J')
        nNum = 12;

    for (int i = 0; i < nNum; i++) {
        for (int j = 0; j < 16; j++)
            temp += sline[i * 16 + 4 + j];

        try { sLine.vValues.push_back(stod(temp)); }

        catch (...) { sLine.vValues.push_back(0); }
        temp = "";
    }

    return sLine;
}

void Cdata::read_o(std::string path) {
    std::ifstream infile(path);
    if (!infile) {
        std::cout << "can't open " << path;
        return;
    }
    std::string line;//暂存每一行
    std::string sTemp = "";//暂存字符串
    int nNum = 0;//暂存循环次数
    struct_line sline;//暂存一行数据
    struct_o sTemp_struct;
    std::istringstream ss(line);
    std::vector<std::string> vTemp_vec;
    while (1) {
        getline(infile, line);
        if (line.find("UTC") != -1) {
            sTemp = line.substr(line.find("HITARGET") + strlen("HITARGET"), line.find("UTC") - (line.find("HITARGET") + strlen("HITARGET")));
            //sTemp = line.substr(line.find("UTC") - 16,16);
            ss.clear();
            ss.str(sTemp);
            ss >> sTemp;
            year = stod(sTemp.substr(0, 4));
            month = stod(sTemp.substr(4, 2));
            day = stod(sTemp.substr(6, 2));
            ss >> sTemp;
            hour = stod(sTemp.substr(0, 2));
            minute = stod(sTemp.substr(2, 2));
            second = stod(sTemp.substr(4, 2));
        }
        else if (line.find("APPROX") != -1) {
            sTemp = line.substr(0, line.find("APPROX"));
            ss.clear();
            ss.str(sTemp);
            ss >> dAPPROX_POSITION[0];
            ss >> dAPPROX_POSITION[1];
            ss >> dAPPROX_POSITION[2];
        }
        else if (line.find("DELTA") != -1) {
            sTemp = line.substr(0, line.find("ANTENNA"));
            ss.str(sTemp);
            ss >> dDELTA[0];
            ss >> dDELTA[1];
            ss >> dDELTA[2];
        }
        else if (line.find("SYS") != -1) {
            sTemp = line.substr(0, line.find("SYS"));
            ss.clear();
            ss.str(sTemp);
            ss >> sTemp;
            vTemp_vec.push_back(sTemp);
            ss >> sTemp;
            vTemp_vec.push_back(sTemp);
            for (int i = 0; i < stod(vTemp_vec[1]); i++) {
                ss >> sTemp;
                vTemp_vec.push_back(sTemp);
            }

            SYS_OBS_TYPES.push_back(vTemp_vec);
            vTemp_vec.clear();
        }
        else if (line.find("END") != -1)
            break;
    }

    while (getline(infile, line)) {
        ss.clear();
        ss.str(line);
        for (int i = 0; i < 9; i++) {
            ss >> sTemp;
            if (i < 7 and i >0) {
                if (sTemp.length() > 4) {
                    if (sTemp[1] != '.') {
                        sTemp_struct.sTime += sTemp[0];
                        sTemp_struct.sTime += sTemp[1];
                    }
                    else {
                        sTemp_struct.sTime += "0";
                        sTemp_struct.sTime += sTemp[0];
                    }
                }
                else
                    sTemp_struct.sTime += sTemp;
            }
            else if (i == 7)
                sTemp_struct.is_error = stoi(sTemp);
            else if (i == 8)
                sTemp_struct.nValue_num = stoi(sTemp);
        }
        for (int i = 0; i < sTemp_struct.nValue_num; i++) {
            getline(infile, line);
            sTemp_struct.vData_o.push_back(Getline_Value_o(line));
        }
        data_o.push_back(sTemp_struct);
        sTemp_struct.vData_o.clear();
        sTemp_struct.sTime = "";
        sTemp_struct.nValue_num = 0;

        sTemp_struct.is_error = 0;
    }

}
std::string Get_time_p(std::string sline) {
    std::string sTime;
    for (int i = 4; i < 24; i++)
        if (sline[i] != ' ')
            sTime += sline[i];
    return sTime;
}
struct_line Get_first_Value_p(std::string sline) {
    struct_line sLine;
    std::string temp;
    for (int i = 0; i < 3; i++)
        sLine.sName += sline[i];
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 19; j++)
            temp += sline[23 + j + 19 * i];
        try {
            if (temp[19 - 3] == '-')
                sLine.vValues.push_back(stod(temp) / (pow(10, 10 * (temp[19 - 2] - '0') + (temp[19 - 1] - '0'))));
            else
                sLine.vValues.push_back(stod(temp) * (pow(10, 10 * (temp[19 - 2] - '0') + (temp[19 - 1] - '0'))));
        }
        catch (...) { sLine.vValues.push_back(0); }
        temp = "";
    }


    return sLine;
}
struct_line Get_other_Value_p(std::string sline, struct_line sLine, int nNum) {
    std::string temp;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 19; j++) {
            if (sLine.vValues.size() == nNum)
                return sLine;
            temp += sline[4 + j + 19 * i];
        }
        try {
            if (temp[19 - 3] == '-')
                sLine.vValues.push_back(stod(temp) / (pow(10, 10 * (temp[19 - 2] - '0') + (temp[19 - 1] - '0'))));
            else
                sLine.vValues.push_back(stod(temp) * (pow(10, 10 * (temp[19 - 2] - '0') + (temp[19 - 1] - '0'))));
        }
        catch (...) { sLine.vValues.push_back(0); }
        temp = "";
    }
    return sLine;
}

void Cdata::read_p(std::string path) {

    std::ifstream infile(path);
    if (!infile) {
        std::cout << "can't open " << path;
        return;
    }
    std::string line;//暂存每一行
    std::string sTemp = "";//暂存字符串
    int nNum = 0;//暂存循环次数
    struct_line sline;//暂存一行数据
    std::string sTime;
    struct_p sTemp_p;
    getline(infile, line);
    std::istringstream ss(line);
    while (1) {
        ss >> sTemp;
        if (sTemp == "END")
            break;
        getline(infile, line);
        ss.clear();
        ss.str(line);
    }
    while (getline(infile, line)) {
        sTemp_p.sTime = Get_time_p(line);
        sline = Get_first_Value_p(line);
        if (sline.sName[0] == 'G')
            nNum = 29;
        else if (sline.sName[0] == 'R')
            nNum = 15;
        else if (sline.sName[0] == 'C')
            nNum = 29;
        else if (sline.sName[0] == 'E')
            nNum = 27;
        else if (sline.sName[0] == 'J')
            nNum = 29;
        while (sline.vValues.size() < nNum) {
            getline(infile, line);
            sline = Get_other_Value_p(line, sline, nNum);
        }
        sTemp_p.vData_p.push_back(sline);
        data_p.push_back(sTemp_p);
        sline.vValues.clear();
        sTemp_p.vData_p.clear();
        nNum = 0;
    }
}

#endif

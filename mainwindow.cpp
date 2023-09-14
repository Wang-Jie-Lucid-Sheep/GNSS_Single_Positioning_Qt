#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QChartView>
#include <QLineSeries>
#include <QValueAxis>
#include "File_Read.h"
#include "Function.h"
/*
#include <iostream>
#include<fstream>  //ifstream
#include <string>
#include <sstream>
#include <vector>
#include <typeinfo>
#include <math.h>
#include <Eigen/Dense>
*/
//#define PI acos(-1)

using namespace std;
//using namespace Eigen;
QT_CHARTS_USE_NAMESPACE
Cdata cdata;
QString fileName_O;
QString fileName_P;
double c = 2.997925e+8 ;//光速
double S_ele_setting = 0;//卫星高度角阈值
MatrixXd ANS(2,2);
MatrixXd POS(2,2);
int geshu=0;
double APPROX_POSITION[3];


int caculate(double lat, double lon,double h, double N, double E, double U, double X, double Y, double Z,Cdata *cdata,int i1,MatrixXd *H,  MatrixXd* L,  MatrixXd* P)
{
    MatrixXd P0(1, 1);//解算权阵
    MatrixXd comp_mat(2, 2);//匹配O与N中相同的卫星
    int g = 1;//记录一个历元内共多少颗可用卫星,g-1为可用卫星数目
    comp_mat.resize(2, (*cdata).get_p_size());
    int middle = 0;
    int best_satellite;
    double JulianDay = YMD_JulianDay((*cdata).get_sData_o(i1).sTime);
    double GPST = JulianDay_GPST(JulianDay);
    double CLK_re = 0;
    for (int i2 = 0; i2 <= (*cdata).get_sData_o(i1).nValue_num - 1; i2++)
    {
        middle = 0;//匹配卫星数
        string PRN_S = (*cdata).get_sData_o(i1).vData_o[i2].sName.substr(0, 1);
        if ((PRN_S == "E") | (PRN_S == "J") | (PRN_S == "R") | (PRN_S == "C"))
        {
            continue;
        }
        for (int i3 = 0; i3 <= (*cdata).get_p_size() - 1; i3++)
        {
            string O_PRN = (*cdata).get_sData_o(i1).vData_o[i2].sName;
            string P_PRN = (*cdata).get_sData_p(i3).vData_p[0].sName;
            if (O_PRN == P_PRN)
            {
                double dtime = abs((*cdata).get_sData_p(i3).vData_p[0].vValues S_TOE - GPST);
                comp_mat(0, middle) = i3;
                comp_mat(1, middle) = dtime;
                middle++;
            }
        }
        int best_satellite_site = Find_best_satellite(middle, &comp_mat);
        best_satellite = comp_mat(0, best_satellite_site);
        double mts = 0;
        if (PRN_S == "G")
        {
            if ((*cdata).get_sData_o(i1).vData_o[i2].get_value("C1C") == -1)
            {
                continue;
            }
            else
            {
                mts = (*cdata).get_sData_o(i1).vData_o[i2].get_value("C1C") / c;
            }
        }
        else if (PRN_S == "C")
        {
            if ((*cdata).get_sData_o(i1).vData_o[i2].get_value("C1I") == -1)
            {
                continue;
            }
            else
            {
                mts = (*cdata).get_sData_o(i1).vData_o[i2].get_value("C1I") / c;
            }
        }
        double t = GPST - mts - CLK_re / c;
        double delta_t = 0;
        double Xk = 0;//卫星空间坐标系X
        double Yk = 0;//卫星空间坐标系Y
        double Zk = 0;//卫星空间坐标系Z
        double R0 = 0;//卫星与站点伪距
        double Satellite_data[29];
        string S_T = (*cdata).get_sData_p(best_satellite).sTime;
        //S_T = S_T + "00";
        double S_JulianDay = YMD_JulianDay(S_T);
        double S_GPST = JulianDay_GPST(S_JulianDay);
        for (int i4 = 0; i4 <= 28; i4++)
        {
            Satellite_data[i4] = (*cdata).get_sData_p(best_satellite).vData_p[0].vValues[i4];
        }
        while (true)
        {

            Satellite_Position(t, best_satellite_site, mts, &delta_t, &Xk, &Yk, &Zk, Satellite_data, PRN_S, S_GPST);
            R0 = sqrt(pow(Xk - X, 2) + pow(Yk - Y, 2) + pow(Zk - Z, 2));
            double ts = R0 / c;
            t = GPST - ts - CLK_re / c;
            if (abs(ts - mts) < 1e-12)
            {
                break;
            }
            mts = ts;
        }
        double  S_ele = Satellite_ele(N, E, U);
        if (S_ele < S_ele_setting)
        {
            continue;
        }
        (*H).conservativeResize(g, Eigen::NoChange);
        (*H)(g - 1, 0) = (X - Xk) / R0;
        (*H)(g - 1, 1) = (Y - Yk) / R0;
        (*H)(g - 1, 2) = (Z - Zk) / R0;
        (*H)(g - 1, 3) = 1;
        double trop_delay = Trop_Delay(S_ele, lat, h);
        (*L).conservativeResize(g, Eigen::NoChange);
        (*L)(g - 1, 0) = (*cdata).get_sData_o(i1).vData_o[i2].get_value("C1C") - R0 + c * delta_t
            + (*cdata).get_sData_p(best_satellite).vData_p[0].vValues S_TGD + trop_delay;
        P0.conservativeResize(Eigen::NoChange, g);
        P0(0, g - 1) = sin(S_ele) / 0.04;
        g += 1;
    }
    if (g - 1 < 4)
    {
        return -1;
    }
    MatrixXd P1 = MatrixXd::Zero(g - 1, g - 1);
    for (int i4 = 0; i4 <= g - 2; i4++)
    {
        P1(i4, i4) = P0(0, i4);
    }
    (*P) = P1;
    return 0;
}


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("GNSS系统单点定位程序");
    setWindowIcon(QIcon(":/new/diqiu.ico"));
    QSound::play(":/new/cszx.wav");
    connect(ui->guanbi,&QAction::triggered,this,&MainWindow::guanbi);
    connect(ui->daoru_O,&QAction::triggered,this,&MainWindow::daoru_O);
    connect(ui->daoru_P,&QAction::triggered,this,&MainWindow::daoru_P);
    connect(ui->baocun,&QAction::triggered,this,&MainWindow::baocun);
    connect(ui->action_2,&QAction::triggered,this,&MainWindow::xinxi);
    connect(ui->Draw_X,&QAction::triggered,this,&MainWindow::Draw_X);
    connect(ui->Draw_Y,&QAction::triggered,this,&MainWindow::Draw_Y);
    connect(ui->Draw_Z,&QAction::triggered,this,&MainWindow::Draw_Z);
    connect(ui->Draw_X_Y,&QAction::triggered,this,&MainWindow::Draw_X_Y);
    ui->progressBar->setRange(0,100);
    ui->progressBar->setValue(0);

}

MainWindow::~MainWindow()
{
    //QString num3;

    delete ui;
}

void MainWindow::on_LSM_Button_clicked()
{
    ui->textBrowser->clear();
    if(cdata.get_o_size()==0|cdata.get_p_size()==0)
    {
        QMessageBox::warning(this,"警告","请先导入文件");
    }
    else
    {
        ui->textBrowser->insertPlainText("开始解算");
        ui->textBrowser->moveCursor(QTextCursor::End);
        ui->textBrowser->append(QString(""));
        ANS.resize(4,cdata.get_o_size());
        POS.resize(3,cdata.get_o_size());
        double lat = 0;//站点大地坐标B
        double lon = 0;//站点大地坐标L
        double h = 0;//站点大地坐标H
        double N = 0;//站心地平坐标系N
        double E = 0;//站心地平坐标系E
        double U = 0;//站心地平坐标系U
        double X = cdata.get_dAPPROX_POSITION(0);//站点空间坐标系X
        double Y = cdata.get_dAPPROX_POSITION(1);//站点空间坐标系Y
        double Z = cdata.get_dAPPROX_POSITION(2);//站点空间坐标系Z
        APPROX_POSITION[0]=X;
        APPROX_POSITION[1]=Y;
        APPROX_POSITION[2]=Z;
        geshu=0;

        XYZ_BLH(X, Y, Z, &lat, &lon, &h);
        BLH_NEU(lat, lon, X, Y, Z, &N, &E, &U);

        for (int i1 = 0; i1 <= cdata.get_o_size()-1; i1++)
        {
            MatrixXd H(1, 4);//解算权阵
            MatrixXd L(1, 1);//解算观测值
            MatrixXd P(2, 2);
            int judge = caculate(lat, lon, h, N, E, U, X, Y, Z, &cdata, i1, &H, &L, &P);
            if(judge==-1)
            {
                if(i1==0)
                {
                    for(int i2=0;i2<=3;i2++)
                    {
                        ANS(i2,i1)=0;
                    }

                    POS(0,i1)=X;
                    POS(1,i1)=Y;
                    POS(2,i1)=Z;
                }
                else
                {
                    for(int i2=0;i2<=3;i2++)
                    {
                        ANS(i2,i1)=ANS(i2,i1-1);
                    }
                    for(int i2=0;i2<=2;i2++)
                    {
                         POS(i2,i1)=POS(i2,i1-1);
                    }
                }

                continue;
            }
            ui->textBrowser->insertPlainText("正在解算第");
            ui->textBrowser->insertPlainText(QString::number(i1+1,'f',0));
            ui->textBrowser->insertPlainText("个历元");
            ui->textBrowser->moveCursor(QTextCursor::End);
            ui->textBrowser->append(QString(""));
            MatrixXd fuzhu(2,2);
            MatrixXd lsm_ans(2, 1);
            fuzhu = H.transpose() * P * H;
            lsm_ans = fuzhu.inverse() * H.transpose() * P * L;
            //double X0, Y0, Z0;
            X = lsm_ans(0)+X;
            Y = lsm_ans(1)+Y;
            Z = lsm_ans(2)+Z;
            for(int i2=0;i2<=3;i2++)
            {
                ANS(i2,i1)=lsm_ans(i2,0);
            }
            POS(0,i1)=X;
            POS(1,i1)=Y;
            POS(2,i1)=Z;
            geshu++;
            for (int a1 = 0; a1 <= 3; a1++)
            {
                //ANS(a1, k) = lsm_ans(a1);
                ui->textBrowser->insertPlainText(QString::number(ANS(a1, i1),'f',6));
                ui->textBrowser->insertPlainText("   ");
            }
            ui->textBrowser->moveCursor(QTextCursor::End);
            ui->textBrowser->append(QString(""));
            for (int a1 = 0; a1 <= 2; a1++)
            {
                 //cout << POS(a1, k) << "   ";
                 ui->textBrowser->insertPlainText(QString::number(POS(a1,i1),'f',6));
                 ui->textBrowser->insertPlainText("   ");
            }
            ui->textBrowser->moveCursor(QTextCursor::End);
            ui->textBrowser->append(QString(""));
            ui->textBrowser->moveCursor(QTextCursor::End);
            ui->textBrowser->append(QString(""));



            int geshu1=(i1+1)*100/cdata.get_o_size();
            ui->progressBar->setValue(geshu1);

        }
    }

}

void MainWindow::on_KALMAN_Button_clicked()
{
    ui->textBrowser->clear();
    if(cdata.get_o_size()==0|cdata.get_p_size()==0)
    {
        QMessageBox::warning(this,"警告","请先导入文件");
    }
    else
    {
        ui->textBrowser->insertPlainText("开始解算");
        ui->textBrowser->moveCursor(QTextCursor::End);
        ui->textBrowser->append(QString(""));
        ANS.resize(4,cdata.get_o_size());
        POS.resize(3,cdata.get_o_size());
        double lat = 0;//站点大地坐标B
        double lon = 0;//站点大地坐标L
        double h = 0;//站点大地坐标H
        double N = 0;//站心地平坐标系N
        double E = 0;//站心地平坐标系E
        double U = 0;//站心地平坐标系U
        double X = cdata.get_dAPPROX_POSITION(0);//站点空间坐标系X
        double Y = cdata.get_dAPPROX_POSITION(1);//站点空间坐标系Y
        double Z = cdata.get_dAPPROX_POSITION(2);//站点空间坐标系Z
        XYZ_BLH(X, Y, Z, &lat, &lon, &h);
        BLH_NEU(lat, lon, X, Y, Z, &N, &E, &U);

        for (int i1 = 0; i1 <= cdata.get_o_size()-1; i1++)
        {
            MatrixXd H(1, 4);//解算权阵
            MatrixXd L(1, 1);//解算观测值
            MatrixXd P(2, 2);
            int judge = caculate(lat, lon, h, N, E, U, X, Y, Z, &cdata, i1, &H, &L, &P);
            if(judge==-1)
            {
                if(i1==0)
                {
                    for(int i2=0;i2<=3;i2++)
                    {
                        ANS(i2,i1)=0;
                    }

                    POS(0,i1)=X;
                    POS(1,i1)=Y;
                    POS(2,i1)=Z;
                }
                else
                {
                    for(int i2=0;i2<=3;i2++)
                    {
                        ANS(i2,i1)=ANS(i2,i1-1);
                    }
                    for(int i2=0;i2<=2;i2++)
                    {
                         POS(i2,i1)=POS(i2,i1-1);
                    }
                }

                continue;
            }

            ui->textBrowser->insertPlainText("正在解算第");
            ui->textBrowser->insertPlainText(QString::number(i1+1,'f',0));
            ui->textBrowser->insertPlainText("个历元");
            ui->textBrowser->moveCursor(QTextCursor::End);
            ui->textBrowser->append(QString(""));


            int geshu=(i1+1)*100/cdata.get_o_size();
            ui->progressBar->setValue(geshu);

        }
    }

}

void MainWindow::on_KALMANCVButton_clicked()
{

    ui->textBrowser->clear();
    if(cdata.get_o_size()==0|cdata.get_p_size()==0)
    {
        QMessageBox::warning(this,"警告","请先导入文件");
    }
    else
    {
        ui->textBrowser->insertPlainText("开始解算");
        ui->textBrowser->moveCursor(QTextCursor::End);
        ui->textBrowser->append(QString(""));
        ANS.resize(4,cdata.get_o_size());
        POS.resize(3,cdata.get_o_size());
        double lat = 0;//站点大地坐标B
        double lon = 0;//站点大地坐标L
        double h = 0;//站点大地坐标H
        double N = 0;//站心地平坐标系N
        double E = 0;//站心地平坐标系E
        double U = 0;//站心地平坐标系U
        double X = cdata.get_dAPPROX_POSITION(0);//站点空间坐标系X
        double Y = cdata.get_dAPPROX_POSITION(1);//站点空间坐标系Y
        double Z = cdata.get_dAPPROX_POSITION(2);//站点空间坐标系Z
        XYZ_BLH(X, Y, Z, &lat, &lon, &h);
        BLH_NEU(lat, lon, X, Y, Z, &N, &E, &U);

        for (int i1 = 0; i1 <= cdata.get_o_size()-1; i1++)
        {
            MatrixXd H(1, 4);//解算权阵
            MatrixXd L(1, 1);//解算观测值
            MatrixXd P(2, 2);
            int judge = caculate(lat, lon, h, N, E, U, X, Y, Z, &cdata, i1, &H, &L, &P);
            if(judge==-1)
            {
                if(i1==0)
                {
                    for(int i2=0;i2<=3;i2++)
                    {
                        ANS(i2,i1)=0;
                    }

                    POS(0,i1)=X;
                    POS(1,i1)=Y;
                    POS(2,i1)=Z;
                }
                else
                {
                    for(int i2=0;i2<=3;i2++)
                    {
                        ANS(i2,i1)=ANS(i2,i1-1);
                    }
                    for(int i2=0;i2<=2;i2++)
                    {
                         POS(i2,i1)=POS(i2,i1-1);
                    }
                }

                continue;
            }
            ui->textBrowser->insertPlainText("正在解算第");
            ui->textBrowser->insertPlainText(QString::number(i1+1,'f',0));
            ui->textBrowser->insertPlainText("个历元");
            ui->textBrowser->moveCursor(QTextCursor::End);
            ui->textBrowser->append(QString(""));


            int geshu=(i1+1)*100/cdata.get_o_size();
            ui->progressBar->setValue(geshu);

        }
    }

}

void MainWindow::on_KALMANCAButton_clicked()
{

    ui->textBrowser->clear();
    if(cdata.get_o_size()==0|cdata.get_p_size()==0)
    {
        QMessageBox::warning(this,"警告","请先导入文件");
    }
    else
    {
        ui->textBrowser->insertPlainText("开始解算");
        ui->textBrowser->moveCursor(QTextCursor::End);
        ui->textBrowser->append(QString(""));
        ANS.resize(4,cdata.get_o_size());
        POS.resize(3,cdata.get_o_size());
        double lat = 0;//站点大地坐标B
        double lon = 0;//站点大地坐标L
        double h = 0;//站点大地坐标H
        double N = 0;//站心地平坐标系N
        double E = 0;//站心地平坐标系E
        double U = 0;//站心地平坐标系U
        double X = cdata.get_dAPPROX_POSITION(0);//站点空间坐标系X
        double Y = cdata.get_dAPPROX_POSITION(1);//站点空间坐标系Y
        double Z = cdata.get_dAPPROX_POSITION(2);//站点空间坐标系Z
        XYZ_BLH(X, Y, Z, &lat, &lon, &h);
        BLH_NEU(lat, lon, X, Y, Z, &N, &E, &U);

        for (int i1 = 0; i1 <= cdata.get_o_size()-1; i1++)
        {
            MatrixXd H(1, 4);//解算权阵
            MatrixXd L(1, 1);//解算观测值
            MatrixXd P(2, 2);
            int judge = caculate(lat, lon, h, N, E, U, X, Y, Z, &cdata, i1, &H, &L, &P);
            if(judge==-1)
            {
                if(i1==0)
                {
                    for(int i2=0;i2<=3;i2++)
                    {
                        ANS(i2,i1)=0;
                    }

                    POS(0,i1)=X;
                    POS(1,i1)=Y;
                    POS(2,i1)=Z;
                }
                else
                {
                    for(int i2=0;i2<=3;i2++)
                    {
                        ANS(i2,i1)=ANS(i2,i1-1);
                    }
                    for(int i2=0;i2<=2;i2++)
                    {
                         POS(i2,i1)=POS(i2,i1-1);
                    }
                }

                continue;
            }

            ui->textBrowser->insertPlainText("正在解算第");
            ui->textBrowser->insertPlainText(QString::number(i1+1,'f',0));
            ui->textBrowser->insertPlainText("个历元");
            ui->textBrowser->moveCursor(QTextCursor::End);
            ui->textBrowser->append(QString(""));


            int geshu=(i1+1)*100/cdata.get_o_size();
            ui->progressBar->setValue(geshu);

        }
    }

}

void MainWindow::lineclear()
{
    ui->lineEdit->clear();
}


void MainWindow::guanbi()
{
    this->close();
}

void MainWindow::daoru_O()
{
    fileName_O = QFileDialog::getOpenFileName(this,"请选择一个O文件",QCoreApplication::applicationFilePath());
    if(fileName_O.isEmpty())
    {
        QMessageBox::warning(this,"警告","请选择一个O文件");
    }
    else
    {
        qDebug()<<fileName_O;
    }
    string filename_O=fileName_O.toStdString();
    cdata.read_o(filename_O);
    ui->textBrowser->insertPlainText("接收机估计坐标位置：");
    ui->textBrowser->moveCursor(QTextCursor::End);
    ui->textBrowser->append(QString(""));
    ui->textBrowser->insertPlainText(QString::number(cdata.get_dAPPROX_POSITION(0),'f',4));
    ui->textBrowser->moveCursor(QTextCursor::End);
    ui->textBrowser->append(QString(""));
    ui->textBrowser->insertPlainText(QString::number(cdata.get_dAPPROX_POSITION(1),'f',4));
    ui->textBrowser->moveCursor(QTextCursor::End);
    ui->textBrowser->append(QString(""));
    ui->textBrowser->insertPlainText(QString::number(cdata.get_dAPPROX_POSITION(2),'f',4));
    ui->textBrowser->moveCursor(QTextCursor::End);
    ui->textBrowser->append(QString(""));
}
void MainWindow::daoru_P()
{
    fileName_P = QFileDialog::getOpenFileName(this,"请选择一个P文件",QCoreApplication::applicationFilePath());
    if(fileName_P.isEmpty())
    {
        QMessageBox::warning(this,"警告","请选择一个P文件");
    }
    else
    {
        qDebug()<<fileName_P;
        string filename_P=fileName_P.toStdString();
        cdata.read_p(filename_P);
        ui->textBrowser->insertPlainText("星历文件导入完成");
        ui->textBrowser->moveCursor(QTextCursor::End);
        ui->textBrowser->append(QString(""));
    }
    //ui->textBrowser->insertPlainText(QString::number(APPROX_POSITION[0],'f',4));
    //ui->textBrowser->moveCursor(QTextCursor::End);
    //ui->textBrowser->append(QString(""));
    //ui->textBrowser->insertPlainText(QString::number(APPROX_POSITION[1],'f',4));
    //ui->textBrowser->moveCursor(QTextCursor::End);
    //ui->textBrowser->append(QString(""));
    //ui->textBrowser->insertPlainText(QString::number(APPROX_POSITION[2],'f',4));
    //ui->textBrowser->moveCursor(QTextCursor::End);
    //ui->textBrowser->append(QString(""));
}

void MainWindow::xinxi()
{
    ui->textBrowser->clear();
    ui->textBrowser->insertPlainText("安徽理工大学");
    ui->textBrowser->moveCursor(QTextCursor::End);
    ui->textBrowser->append(QString(""));
    ui->textBrowser->insertPlainText("导航工程20-2");
    ui->textBrowser->moveCursor(QTextCursor::End);
    ui->textBrowser->append(QString(""));
    ui->textBrowser->insertPlainText("王杰");
    ui->textBrowser->moveCursor(QTextCursor::End);
    ui->textBrowser->append(QString(""));

}

void MainWindow::baocun()
{


}


void MainWindow::Draw_X()
{

    if(POS.size()==4)
    {
        QMessageBox::warning(this,"警告","请先进行解算");
    }
    else
    {
    drawxx *drawxxw=new drawxx(POS,geshu,APPROX_POSITION);
    //Form *formw = new Form(POS,geshu,APPROX_POSITION);
    //formw=new Form;
    drawxxw->show();
    }

}

void MainWindow::Draw_Y()
{

    if(POS.size()==4)
    {
        QMessageBox::warning(this,"警告","请先进行解算");
    }
    else
    {
    drawyy *drawyyw=new drawyy(POS,geshu,APPROX_POSITION);
    //Form *formw = new Form(POS,geshu,APPROX_POSITION);
    //formw=new Form;
    drawyyw->show();
    }

}

void MainWindow::Draw_Z()
{

    if(POS.size()==4)
    {
        QMessageBox::warning(this,"警告","请先进行解算");
    }
    else
    {
    drawzz *drawzzw=new drawzz(POS,geshu,APPROX_POSITION);
    //Form *formw = new Form(POS,geshu,APPROX_POSITION);
    //formw=new Form;
    drawzzw->show();
    }

}

void MainWindow::Draw_X_Y()
{

    if(POS.size()==4)
    {
        QMessageBox::warning(this,"警告","请先进行解算");
    }
    else
    {
    drawxy *drawxyw=new drawxy(POS,geshu,APPROX_POSITION);
    //Form *formw = new Form(POS,geshu,APPROX_POSITION);
    //formw=new Form;
     drawxyw->show();
    }

}




void MainWindow::on_Config_Button_clicked()
{
    Config *Config_show=new Config();
    Config_show->show();
}

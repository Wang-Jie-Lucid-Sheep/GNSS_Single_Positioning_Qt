#include "drawxy.h"
#include "ui_drawxy.h"
#include <QChartView>
#include <QLineSeries>
#include <QValueAxis>
#include <qmath.h>
#include <QMessageBox>
QT_CHARTS_USE_NAMESPACE
drawxy::drawxy(Eigen::MatrixXd POS1, int geshu1, double *APPROX1, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::drawxy)
{
    ui->setupUi(this);
    this->setWindowTitle("X-Y绘图");
    setWindowIcon(QIcon(":/new/diqiu.ico"));
    POS=POS1;geshu=geshu1;
    //qDebug()<<POS.size()<<endl;

    //qDebug()<<APPROX1[1]<<endl;
    //qDebug()<<POS1(0,0)<<endl;
    //qDebug()<<POS(0,0)<<endl;
    QChartView* xdraw=new QChartView(this);
    QChart* chart=new QChart();
    xdraw->setChart(chart);
    setCentralWidget(xdraw);
    //formw->show();

    QLineSeries* Drawx=new QLineSeries;
    Drawx->setName("X-Y坐标");
    chart->addSeries(Drawx);
    for(qreal a1=0;a1<=geshu-1;a1++)
    {
        //Drawx->append(a1,(1));//POS(0,a1)/1000000)
        //*series << QPointF(11, 1)
        *Drawx<<QPointF(POS(0,a1),POS(1,a1));
    }

    int APPROX2=APPROX1[0];
    int APPROX3=APPROX1[1];

    QValueAxis* axisX=new QValueAxis;
    axisX->setRange(APPROX2-200,APPROX2+200);
    chart->setAxisX(axisX,Drawx);

    QValueAxis* axisY=new QValueAxis;
    axisY->setRange(APPROX3-200,APPROX3+200);//(APPROX1[0]+15)/1000000,(APPROX1[0]-15)/1000000
    chart->setAxisY(axisY,Drawx);
}

drawxy::~drawxy()
{
    delete ui;
}

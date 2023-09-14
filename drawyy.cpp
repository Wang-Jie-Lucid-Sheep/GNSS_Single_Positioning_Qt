#include "drawyy.h"
#include "ui_drawyy.h"
#include <QChartView>
#include <QLineSeries>
#include <QValueAxis>
#include <qmath.h>
#include <QMessageBox>
QT_CHARTS_USE_NAMESPACE
drawyy::drawyy(Eigen::MatrixXd POS1, int geshu1, double *APPROX1, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::drawyy)
{
    ui->setupUi(this);
    this->setWindowTitle("Y绘图");
    setWindowIcon(QIcon(":/new/diqiu.ico"));
    POS=POS1;geshu=geshu1;
    //qDebug()<<POS.size()<<endl;

    //qDebug()<<APPROX1[1]<<endl;
    //qDebug()<<POS1(0,0)<<endl;
    //qDebug()<<POS(0,0)<<endl;
    QChartView* ydraw=new QChartView(this);
    QChart* chart=new QChart();
    ydraw->setChart(chart);
    setCentralWidget(ydraw);
    //formw->show();

    QLineSeries* Drawy=new QLineSeries;
    Drawy->setName("Y坐标");
    chart->addSeries(Drawy);
    for(qreal a1=0;a1<=geshu-1;a1++)
    {
        //Drawy->append(a1,(1));//POS(0,a1)/1000000)
        //*series << QPointF(11, 1)
        *Drawy<<QPointF(a1,POS(1,a1));
    }
    QValueAxis* axisX=new QValueAxis;
    axisX->setRange(1,geshu);
    chart->setAxisX(axisX,Drawy);

    int APPROX2=APPROX1[1];

    QValueAxis* axisY=new QValueAxis;
    axisY->setRange(APPROX2-100,APPROX2+100);//(APPROX1[0]+15)/1000000,(APPROX1[0]-15)/1000000
    chart->setAxisY(axisY,Drawy);
}

drawyy::~drawyy()
{
    delete ui;
}

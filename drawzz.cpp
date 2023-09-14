#include "drawzz.h"
#include "ui_drawzz.h"
#include <QChartView>
#include <QLineSeries>
#include <QValueAxis>
#include <qmath.h>
#include <QMessageBox>
QT_CHARTS_USE_NAMESPACE
drawzz::drawzz(Eigen::MatrixXd POS1, int geshu1, double *APPROX1, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::drawzz)
{
    ui->setupUi(this);
    this->setWindowTitle("Z绘图");
    setWindowIcon(QIcon(":/new/diqiu.ico"));
    POS=POS1;geshu=geshu1;
    //qDebug()<<POS.size()<<endl;

    //qDebug()<<APPROX1[1]<<endl;
    //qDebug()<<POS1(0,0)<<endl;
    //qDebug()<<POS(0,0)<<endl;
    QChartView* zdraw=new QChartView(this);
    QChart* chart=new QChart();
    zdraw->setChart(chart);
    setCentralWidget(zdraw);
    //formw->show();

    QLineSeries* Drawz=new QLineSeries;
    Drawz->setName("Z坐标");
    chart->addSeries(Drawz);
    for(qreal a1=0;a1<=geshu-1;a1++)
    {
        //Drawz->append(a1,(1));//POS(0,a1)/1000000)
        //*series << QPointF(11, 1)
        *Drawz<<QPointF(a1,POS(2,a1));
    }
    QValueAxis* axisX=new QValueAxis;
    axisX->setRange(1,geshu);
    chart->setAxisX(axisX,Drawz);

    int APPROX2=APPROX1[2];

    QValueAxis* axisY=new QValueAxis;
    axisY->setRange(APPROX2-100,APPROX2+100);//(APPROX1[0]+15)/1000000,(APPROX1[0]-15)/1000000
    chart->setAxisY(axisY,Drawz);
}

drawzz::~drawzz()
{
    delete ui;
}

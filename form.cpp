#include "form.h"
#include "ui_form.h"
#include <QChartView>
#include <QLineSeries>
#include <QValueAxis>
//#include "mainwindow.h"
QT_CHARTS_USE_NAMESPACE
Form::Form(Eigen::MatrixXd POS1, int geshu1, double *APPROX1, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form)
{
    ui->setupUi(this);

    POS=POS1;geshu=geshu1;

    QChartView* xdraw=new QChartView(this);
    QChart* chart=new QChart();
    xdraw->setChart(chart);
    //setCentralWidget(xdraw);
    //formw->show();

    QLineSeries* Drawx=new QLineSeries;
    Drawx->setName("X坐标");
    chart->addSeries(Drawx);
    for(int a1=0;a1<=geshu-1;a1++)
    {
        Drawx->append(a1,POS(0,a1));
    }
    QValueAxis* axisX=new QValueAxis;
    axisX->setRange(1,geshu);
    chart->setAxisX(axisX,Drawx);

    QValueAxis* axisY=new QValueAxis;
    axisY->setRange(APPROX1[0]+15,APPROX1[0]-15);
    chart->setAxisY(axisY,Drawx);

}

Form::~Form()
{
    delete ui;
}

#include "drawxx.h"
#include "ui_drawxx.h"
#include <QChartView>
#include <QLineSeries>
#include <QValueAxis>
#include <qmath.h>
#include <QMessageBox>
QT_CHARTS_USE_NAMESPACE
drawxx::drawxx(Eigen::MatrixXd POS1, int geshu1, double *APPROX1, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::drawxx)
{
    ui->setupUi(this);
    this->setWindowTitle("X绘图");
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
    Drawx->setName("X坐标");
    chart->addSeries(Drawx);
    for(qreal a1=0;a1<=geshu-1;a1++)
    {
        //Drawx->append(a1,(1));//POS(0,a1)/1000000)
        //*series << QPointF(11, 1)
        *Drawx<<QPointF(a1,POS(0,a1));
    }
    QValueAxis* axisX=new QValueAxis;
    axisX->setRange(1,geshu);
    chart->setAxisX(axisX,Drawx);

    int APPROX2=APPROX1[0];

    QValueAxis* axisY=new QValueAxis;
    axisY->setRange(APPROX2-100,APPROX2+100);//(APPROX1[0]+15)/1000000,(APPROX1[0]-15)/1000000
    chart->setAxisY(axisY,Drawx);

    /*
    setWindowTitle("QChart简单绘图");

        //创建图表框架
        QChartView* cview = new QChartView(this);
        QChart* chart = new QChart();
        chart->setTitle("简单函数曲线");

        cview->setChart(chart);
        setCentralWidget(cview);

        //创建折现序列
        QLineSeries* seriesS = new QLineSeries;
        QLineSeries* seriesC = new QLineSeries;
        seriesS->setName("Sinθ");
        seriesC->setName("Cosθ");
        chart->addSeries(seriesS);
        chart->addSeries(seriesC);

        //添加数据绘制
        qreal y0,y1,t=0,intv=0.1;
        size_t count=100;
        for (size_t i=0;i<count;i++) {
            y0 = qSin(t);
            y1 = qCos(t);
            seriesS->append(t,y0);
            seriesC->append(t,y1);
            t += intv;
        }

        //创建坐标轴

        QValueAxis* axisX = new QValueAxis;
        axisX->setRange(0,100);
        chart->setAxisX(axisX,seriesS);
        chart->setAxisX(axisX,seriesC);

        QValueAxis* axisY = new QValueAxis;
        axisY->setRange(-1, 1);
        chart->setAxisY(axisY,seriesS);
        chart->setAxisY(axisY,seriesC);
*/
}

drawxx::~drawxx()
{
    delete ui;
}

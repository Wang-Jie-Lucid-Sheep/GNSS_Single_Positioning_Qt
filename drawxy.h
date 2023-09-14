#ifndef DRAWXY_H
#define DRAWXY_H

#include <QMainWindow>
#include <Eigen/Dense>
#include <iostream>
#include <QDebug>
namespace Ui {
class drawxy;
}

class drawxy : public QMainWindow
{
    Q_OBJECT

public:
    explicit drawxy(Eigen::MatrixXd POS1, int geshu1, double *APPROX1, QWidget *parent = 0);
    ~drawxy();

private:
    Ui::drawxy *ui;
    Eigen::MatrixXd POS;
    int geshu;
    double *APPROX;
};

#endif // DRAWXY_H

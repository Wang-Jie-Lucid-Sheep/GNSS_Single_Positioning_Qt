#ifndef DRAWXX_H
#define DRAWXX_H

#include <QMainWindow>
#include <Eigen/Dense>
#include <iostream>
#include <QDebug>
namespace Ui {
class drawxx;
}

class drawxx : public QMainWindow
{
    Q_OBJECT

public:
    explicit drawxx(Eigen::MatrixXd POS1, int geshu1, double *APPROX1, QWidget *parent = 0);
    ~drawxx();

private:
    Ui::drawxx *ui;
    Eigen::MatrixXd POS;
    int geshu;
    double *APPROX;
};

#endif // DRAWXX_H

#ifndef DRAWZZ_H
#define DRAWZZ_H

#include <QMainWindow>
#include <Eigen/Dense>
#include <iostream>

namespace Ui {
class drawzz;
}

class drawzz : public QMainWindow
{
    Q_OBJECT

public:
    explicit drawzz(Eigen::MatrixXd POS1, int geshu1, double *APPROX1, QWidget *parent = 0);
    ~drawzz();

private:
    Ui::drawzz *ui;
    Eigen::MatrixXd POS;
    int geshu;
    double *APPROX;
};

#endif // DRAWZZ_H

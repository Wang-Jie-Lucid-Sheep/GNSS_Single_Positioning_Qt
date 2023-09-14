#ifndef DRAWYY_H
#define DRAWYY_H

#include <QMainWindow>
#include <Eigen/Dense>
#include <iostream>
namespace Ui {
class drawyy;
}

class drawyy : public QMainWindow
{
    Q_OBJECT

public:
    explicit drawyy(Eigen::MatrixXd POS1, int geshu1, double *APPROX1, QWidget *parent = 0);
    ~drawyy();

private:
    Ui::drawyy *ui;
    Eigen::MatrixXd POS;
    int geshu;
    double *APPROX;
};

#endif // DRAWYY_H

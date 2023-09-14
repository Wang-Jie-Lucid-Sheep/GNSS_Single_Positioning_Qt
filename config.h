#ifndef CONFIG_H
#define CONFIG_H

#include <QMainWindow>

namespace Ui {
class Config;
}

class Config : public QMainWindow
{
    Q_OBJECT

public:
    explicit Config(QWidget *parent = 0);
    ~Config();

private slots:
    void on_cancel_button_clicked();

    void on_OK_Button_clicked();

private:
    Ui::Config *ui;
};

#endif // CONFIG_H

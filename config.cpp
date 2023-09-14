#include "config.h"
#include "ui_config.h"

Config::Config(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Config)
{
    ui->setupUi(this);
    this->setWindowTitle("设置");
    setWindowIcon(QIcon(":/new/diqiu.ico"));
}

Config::~Config()
{
    delete ui;
}

void Config::on_cancel_button_clicked()
{
    this->close();
}

void Config::on_OK_Button_clicked()
{
    this->close();
}

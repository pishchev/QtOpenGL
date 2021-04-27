#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QObject>
#include <QSlider>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QSurfaceFormat format;
    format.setSamples(16);
    format.setVersion(3,3);
    ui->CoubToCircle->setFormat(format);
    QObject::connect(ui->Color,&QPushButton::clicked,this, &MainWindow::requestColor);
    QObject::connect(ui->FragFactor,&QSlider::valueChanged,this, &MainWindow::requestPercistanceLvl);
    QObject::connect(ui->CtSFactor,&QSlider::valueChanged,this, &MainWindow::requestFactor);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::requestPercistanceLvl()
{
    auto val = ui->FragFactor->value();
    ui->CoubToCircle->setPercistenceLvl(val);
}

 void MainWindow::requestColor()
 {
     color.show();
     color.move(this->x() + this->width(),this->y());
     color.setCurrentColor(ui->CoubToCircle->getColor());
     ui->CoubToCircle->setColor(&color);
 }

 void MainWindow::requestFactor()
 {
     ui->CoubToCircle->setFactor( static_cast<float>(ui->CtSFactor->value())/100);
 }

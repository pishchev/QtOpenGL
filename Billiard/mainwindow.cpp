#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QObject>
#include <QSlider>
#include <QLabel>
#include <QCheckBox>
#include <vector>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QSurfaceFormat format;
    format.setSamples(16);
    format.setVersion(3,3);
    ui->BilliardWidget->setFormat(format);

    requestLightFactors();

    QObject::connect(ui->AmbientFactor,&QSlider::valueChanged,this, &MainWindow::requestLightFactors);
    QObject::connect(ui->DiffuseFactor,&QSlider::valueChanged,this, &MainWindow::requestLightFactors);
    QObject::connect(ui->KFactor,&QSlider::valueChanged,this, &MainWindow::requestLightFactors);
    QObject::connect(ui->PFactor,&QSlider::valueChanged,this, &MainWindow::requestLightFactors);
    QObject::connect(ui->N,&QSlider::valueChanged,this, &MainWindow::requestLightFactors);
    QObject::connect(ui->SpecularFactor,&QSlider::valueChanged,this, &MainWindow::requestLightFactors);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::requestLightFactors()
{
    ui->BilliardWidget->setLightCoefs({
                                 ui->AmbientFactor->value(),
                                 ui->DiffuseFactor->value(),
                                 ui->KFactor->value(),
                                 ui->PFactor->value(),
                                 ui->N->value(),
                                 ui->SpecularFactor->value()
                             });
}


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
    ui->Coubs->setFormat(format);

    QObject::connect(ui->ColorAmbient,&QPushButton::clicked,this, &MainWindow::requestColorAm);
    QObject::connect(ui->ColorDirection,&QPushButton::clicked,this, &MainWindow::requestColorDir);
    QObject::connect(ui->ColorPoint,&QPushButton::clicked,this, &MainWindow::requestColorPnt);

    QObject::connect(ui->RT_Flag,&QCheckBox::clicked,this, &MainWindow::requestRotateFlag);

    QObject::connect(ui->AmbientFactor,&QSlider::valueChanged,this, &MainWindow::requestLightFactors);
    QObject::connect(ui->DiffuseFactor,&QSlider::valueChanged,this, &MainWindow::requestLightFactors);
    QObject::connect(ui->KFactor,&QSlider::valueChanged,this, &MainWindow::requestLightFactors);
    QObject::connect(ui->PFactor,&QSlider::valueChanged,this, &MainWindow::requestLightFactors);

    QObject::connect(ui->DL_Flag,&QCheckBox::clicked,this, &MainWindow::requestActivate);
    QObject::connect(ui->PL_Flag,&QCheckBox::clicked,this, &MainWindow::requestActivate);

    QObject::connect(ui->DLX,&QSlider::valueChanged,this, &MainWindow::requestDirPos);
    QObject::connect(ui->DLY,&QSlider::valueChanged,this, &MainWindow::requestDirPos);
    QObject::connect(ui->DLZ,&QSlider::valueChanged,this, &MainWindow::requestDirPos);

    QObject::connect(ui->PLX,&QSlider::valueChanged,this, &MainWindow::requestPntPos);
    QObject::connect(ui->PLY,&QSlider::valueChanged,this, &MainWindow::requestPntPos);
    QObject::connect(ui->PLZ,&QSlider::valueChanged,this, &MainWindow::requestPntPos);

    QObject::connect(ui->Z,&QSlider::valueChanged,this, &MainWindow::requestPosition);
    QObject::connect(ui->Y,&QSlider::valueChanged,this, &MainWindow::requestPosition);
    QObject::connect(ui->X,&QSlider::valueChanged,this, &MainWindow::requestPosition);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::requestActivate()
{
    std::vector<bool> vec;
    vec.push_back(ui->DL_Flag->checkState());
    vec.push_back(ui->PL_Flag->checkState());
    ui->Coubs->setActivateLight(vec);
}

void MainWindow::requestPosition()
{
    ui->Coubs->setPosition(float( ui->X->value() )/100 ,float( ui->Y->value())/100, float(ui->Z->value())/100);
}

void MainWindow::requestColor(int i)
{
    color.show();
    color.move(this->x(),this->y() + this->height()/ 2);
    ui->Coubs->setColor(&color , i);
}

void MainWindow::requestRotateFlag()
{
    ui->Coubs->setRotate(ui->RT_Flag->checkState());
}
void MainWindow::requestLightFactors()
{
    ui->Coubs->setLightCoefs({
                                 ui->AmbientFactor->value(),
                                 ui->DiffuseFactor->value(),
                                 ui->KFactor->value(),
                                 ui->PFactor->value(),
                             });
}

void MainWindow::requestColorAm()
{
    requestColor(0);
}
void MainWindow::requestColorDir()
{
    requestColor(1);
}
void MainWindow::requestColorPnt()
{
    requestColor(2);
}
void MainWindow::requestColorPrj()
{
    requestColor(3);
}
void MainWindow::requestDirPos()
{
    ui->Coubs->setDirectionLightPosition(QVector4D(ui->DLX->value(),ui->DLY->value(),ui->DLZ->value() ,1)) ;
}
void MainWindow::requestPntPos()
{
    ui->Coubs->setPointLightPosition(QVector4D(ui->PLX->value(),ui->PLY->value(),ui->PLZ->value() ,1));
}


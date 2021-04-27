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
    QObject::connect(ui->ColorProj,&QPushButton::clicked,this, &MainWindow::requestColorPrj);

    QObject::connect(ui->FragFactor,&QSlider::valueChanged,this, &MainWindow::requestPercistanceLvl);
    QObject::connect(ui->Cols,&QSlider::valueChanged,this, &MainWindow::requestCol);
    QObject::connect(ui->Rows,&QSlider::valueChanged,this, &MainWindow::requestRow);

    QObject::connect(ui->RX,&QSlider::valueChanged,this, &MainWindow::requestRotation);
    QObject::connect(ui->RY,&QSlider::valueChanged,this, &MainWindow::requestRotation);
    QObject::connect(ui->RZ,&QSlider::valueChanged,this, &MainWindow::requestRotation);

    QObject::connect(ui->RT_Flag,&QCheckBox::clicked,this, &MainWindow::requestRotateFlag);

    QObject::connect(ui->AmbientFactor,&QSlider::valueChanged,this, &MainWindow::requestLightFactors);
    QObject::connect(ui->DiffuseFactor,&QSlider::valueChanged,this, &MainWindow::requestLightFactors);
    QObject::connect(ui->SpecularFactor,&QSlider::valueChanged,this, &MainWindow::requestLightFactors);
    QObject::connect(ui->Nparam,&QSlider::valueChanged,this, &MainWindow::requestLightFactors);
    QObject::connect(ui->KFactor,&QSlider::valueChanged,this, &MainWindow::requestLightFactors);
    QObject::connect(ui->PFactor,&QSlider::valueChanged,this, &MainWindow::requestLightFactors);

    QObject::connect(ui->DL_Flag,&QCheckBox::clicked,this, &MainWindow::requestActivate);
    QObject::connect(ui->PL_Flag,&QCheckBox::clicked,this, &MainWindow::requestActivate);
    QObject::connect(ui->PjL_Flag,&QCheckBox::clicked,this, &MainWindow::requestActivate);

    QObject::connect(ui->DLX,&QSlider::valueChanged,this, &MainWindow::requestDirPos);
    QObject::connect(ui->DLY,&QSlider::valueChanged,this, &MainWindow::requestDirPos);
    QObject::connect(ui->DLZ,&QSlider::valueChanged,this, &MainWindow::requestDirPos);

    QObject::connect(ui->PLX,&QSlider::valueChanged,this, &MainWindow::requestPntPos);
    QObject::connect(ui->PLY,&QSlider::valueChanged,this, &MainWindow::requestPntPos);
    QObject::connect(ui->PLZ,&QSlider::valueChanged,this, &MainWindow::requestPntPos);

    QObject::connect(ui->PrX,&QSlider::valueChanged,this, &MainWindow::requestProj);
    QObject::connect(ui->PrY,&QSlider::valueChanged,this, &MainWindow::requestProj);
    QObject::connect(ui->PrZ,&QSlider::valueChanged,this, &MainWindow::requestProj);
    QObject::connect(ui->PrRX,&QSlider::valueChanged,this, &MainWindow::requestProj);
    QObject::connect(ui->PrRY,&QSlider::valueChanged,this, &MainWindow::requestProj);
    QObject::connect(ui->PrRZ,&QSlider::valueChanged,this, &MainWindow::requestProj);
    QObject::connect(ui->PrAngle,&QSlider::valueChanged,this, &MainWindow::requestProj);
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
    vec.push_back(ui->PjL_Flag->checkState());
    ui->Coubs->setActivateLight(vec);
}
void MainWindow::requestPercistanceLvl()
{
    auto val = ui->FragFactor->value();
    ui->Coubs->setPercistenceLvl(val);
}
void MainWindow::requestColor(int i)
{
    color.show();
    color.move(this->x(),this->y() + this->height()/ 2);
    ui->Coubs->setColor(&color , i);
}
void MainWindow::requestRow()
{
    auto val = ui->Rows->value();
    ui->Coubs->setRow(val);
    requestRotation();
    requestRotateFlag();
}
void MainWindow::requestCol()
{
    auto val = ui->Cols->value();
    ui->Coubs->setCol(val);
    requestRotation();
    requestRotateFlag();
}
void MainWindow::requestRotation()
{
    ui->Coubs->setRotation({static_cast<float>(ui->RX->value()) ,static_cast<float>(ui->RY->value()),static_cast<float>(ui->RZ->value()) });
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
                                 ui->SpecularFactor->value(),
                                 ui->Nparam->value()

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

void MainWindow::requestProj()
{
    ui->Coubs->setProjectLight(
                QVector4D(ui->PrX->value(),ui->PrY->value(),ui->PrZ->value(),1),
                QVector4D(ui->PrRX->value(),ui->PrRY->value(),ui->PrRZ->value(),1),
                3.14f/ui->PrAngle->value()
                );
}

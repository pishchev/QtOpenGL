#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QColorDialog>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void requestFactor();
    void requestPercistanceLvl();
    void requestColor();
private:
    QColorDialog color;
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H

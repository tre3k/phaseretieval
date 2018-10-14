#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGridLayout>

#include "tplot2dcase.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    tPlot2DCase *plotIn;
    tPlot2DCase *plotOut;
    tPlot2DCase *plotInAmpl;
    tPlot2DCase *plotInPhase;
    tPlot2DCase *plotOutAmpl;
    tPlot2DCase *plotOutPhase;


private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H

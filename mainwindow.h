#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGridLayout>

#include "tcomplex.h"
#include "tplot2dcase.h"
#include "processthread.h"
#include "tfft.h"

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

    tComplex2D InputImage;


private slots:
    void on_actionExit_triggered();

    void on_actionProcess_triggered();

    void on_actionOpen_image_triggered();

    void OpenImage(QString filename, tComplex2D *output);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H

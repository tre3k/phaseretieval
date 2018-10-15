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

    QProgressBar *progressBar;

    tComplex2D InputImage;
    tComplex2D measure;

    ProcessThread *process;


private slots:
    void on_actionExit_triggered();

    void on_actionProcess_triggered();

    void on_actionOpen_image_triggered();

    void OpenImage(QString filename, tComplex2D *output);

    void on_actionOpen_Amplitude_triggered();
    void ReadSANS1(tComplex2D *data, QString filename);

    void on_actionOpen_SANS_1_data_triggered();

public slots:
    void slot_showMessage(QString);
    void slot_setProgress(int);

    void slot_plotResult(tComplex2D *);
    void slot_plotOutAmpl(tComplex2D *);
    void slot_plotOutPhase(tComplex2D *);

signals:
    void signal_send_data_process(s_data_process *);

private:
    int counter = 0;
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H

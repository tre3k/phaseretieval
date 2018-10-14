#ifndef PROCESSTHREAD_H
#define PROCESSTHREAD_H

#include <QThread>

#include "tplot2dcase.h"
#include "tfft.h"
#include "tcomplex2d.h"

struct s_data_process{
    int n_itteration;
    tComplex2D *inputData;
    double betta;

};

class ProcessThread : public QThread{
    Q_OBJECT
public:
    explicit ProcessThread(QObject *parent = nullptr);
    void run();

private:
    s_data_process *dp;

    tComplex2D error_reduction(tComplex2D *input, tComplex2D *space_constr, int n_itteration);

    void generateProbeCircle(tComplex2D *data,double r,double x,double y);
    void generateProbeSquare(tComplex2D *data,double center_x,double center_y,double w,double h);
    int doubleToInt(double);

private slots:
    void slot_recive_data_process(s_data_process *);

signals:
    void signal_showMessage(QString);
    void signal_setProgress(int);

    void signal_plotResult(tComplex2D *);
    void signal_plotAmpl(tComplex2D *);
    void signal_plotPhase(tComplex2D *);

};


#endif

#ifndef PROCESSTHREAD_H
#define PROCESSTHREAD_H

#include <QThread>

#include "tplot2dcase.h"
#include "tfft.h"
#include "tcomplex2d.h"
#include "plotdialog.h"

enum{
    METHOD_ERROR_REDUCTION,
    METHOD_HYBRID_IN_OUT
};

enum{
    PROBE_FUNCTION_SQUARE,
    PROBE_FUNCTION_CIRCLE
};


struct s_data_process{
    int n_itteration;
    tComplex2D *inputData;
    double betta;
    double treshold;
    double background;
    int method;
    int probe;

    double probeParam1;
    double probeParam2;
    double probeParam3;
    double probeParam4;

    PlotDialog *dialogErrors;

    int n_of_plots;
};

class ProcessThread : public QThread{
    Q_OBJECT
public:
    explicit ProcessThread(QObject *parent = nullptr);
    void run();

private:
    s_data_process *dp;
    bool ready = true;

    tComplex2D error_reduction(tComplex2D *input, tComplex2D *space_constr, int n_itteration);
    tComplex2D hybrid_input_output(tComplex2D *input, tComplex2D *space_constr, double betta,int n_itteration);

    double findError(tComplex2D *ideal,tComplex2D *compare);

    void generateProbeCircle(tComplex2D *data,double r,double x,double y);
    void generateProbeSquare(tComplex2D *data,double center_x,double center_y,double w,double h);
    int doubleToInt(double);

    void plotGraphs(tComplex2D *x,tComplex2D *X, tComplex2D *F,int i);

private slots:
    void slot_recive_data_process(s_data_process *);
    void slot_ready();

signals:
    void signal_showMessage(QString);
    void signal_setProgress(int);

    void signal_plotResult(tComplex2D *);
    void signal_plotAmpl(tComplex2D *);
    void signal_plotPhase(tComplex2D *);

};


#endif

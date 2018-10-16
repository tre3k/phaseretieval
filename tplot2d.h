#ifndef TPLOT2D_H
#define TPLOT2D_H

#include "qcustomplot.h"
#include "tplot.h"
#include "tcomplex2d.h"

enum{
    PLOT2D_COMPLEX2D_AMPLITUDE,
    PLOT2D_COMPLEX2D_PHASE,
    PLOT2D_COMPLEX2D_REAL,
    PLOT2D_COMPLEX2D_IMGN
};

class tPlot2D : public tPlot{
    Q_OBJECT
public:
    explicit tPlot2D(QWidget *parent = 0);
    QCPColorMap *ColorMap;
    QCPColorScale *ColorScale;

public slots:
    void plot2DMap(double **data,int size_x,int size_y);
    void plotComplex2DMap(tComplex2D,int param);
    void plotComplex2DMapWithSort(tComplex2D,int param);

signals:
    void signal_ready();
};

#endif

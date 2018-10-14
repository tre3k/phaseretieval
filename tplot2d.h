#ifndef TPLOT2D_H
#define TPLOT2D_H

#include "qcustomplot.h"
#include "tplot.h"

class tPlot2D : public tPlot{
    Q_OBJECT
public:
    explicit tPlot2D(QWidget *parent = 0);
    QCPColorMap *ColorMap;
    QCPColorScale *ColorScale;
};

#endif

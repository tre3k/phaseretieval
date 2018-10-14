#ifndef TPLOT_H
#define TPLOT_H

#include "qcustomplot.h"

class tPlot : public QCustomPlot{
    Q_OBJECT
public:
    explicit tPlot(QWidget *parent = 0);
};


#endif

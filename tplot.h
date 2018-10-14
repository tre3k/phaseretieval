#ifndef TPLOT_H
#define TPLOT_H

#include "qcustomplot.h"

class tPlot : public QCustomPlot{
    Q_OBJECT
public:
    explicit tPlot(QWidget *parent = 0);

public slots:
    void slot_sAxies_drag_zoom(QCPAxis* sAxis,QCPAxis::SelectablePart part,QMouseEvent* event);
    void slot_full_drag_zoom(QMouseEvent *mouseEvent);
    void slot_selectionChanged(void);
};


#endif

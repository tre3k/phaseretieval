#ifndef TPLOT_H
#define TPLOT_H

#include <QVector>
#include "qcustomplot.h"


class tPlot : public QCustomPlot{
    Q_OBJECT
public:
    explicit tPlot(QWidget *parent = 0);
    void addNewValue(double valueX,double valueY,int graph);
    void clearData();
    int createGraph(QColor colorLine="green");
    void clearGraph(int graph);


public slots:
    void slot_sAxies_drag_zoom(QCPAxis* sAxis,QCPAxis::SelectablePart part,QMouseEvent* event);
    void slot_full_drag_zoom(QMouseEvent *mouseEvent);
    void slot_selectionChanged(void);
    void slot_addNewValue(double valueX,double valueY,int graph);
    void slot_clearData();

private:
    QVector<double> dataX,dataY;
    int graph_count;

signals:
    void signal_ready();
};


#endif

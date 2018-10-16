#ifndef PLOTDIALOG_H
#define PLOTDIALOG_H

#include <QDialog>
#include <QWidget>
#include "tplot.h"

class PlotDialog : public QDialog{
    Q_OBJECT
public:
    explicit PlotDialog(QWidget *parent = 0);
    tPlot *plot;

    void setAxiesLabel(QString labelX,QString LabelY);
    void addValue(double x,double y);
    void setGraph(int);

private:
    int graph;
    QVBoxLayout *mainLayout;

private slots:
    void slot_SetAxiesLabel(QString labelX,QString LabelY);
    void slot_addValue(double x,double y);

};

#endif

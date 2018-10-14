#ifndef TPLOT2DCASE_H
#define TPLOT2DCASE_H

#include <QWidget>
#include <QCheckBox>
#include <QVBoxLayout>
#include <QHBoxLayout>

#include "qcustomplot.h"
#include "tplot.h"
#include "tplot2d.h"

class tPlot2DCase : public QWidget{
    Q_OBJECT
public:
    explicit tPlot2DCase(QWidget *parent = 0);
    tPlot2D *plot2D;
    QCheckBox *checkBoxLog;
    QCheckBox *checkBoxHold;
    QVBoxLayout *vLayout;
    QHBoxLayout *hLayout;

public slots:
    void slot_log(bool);

};


#endif

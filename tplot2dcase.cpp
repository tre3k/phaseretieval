#include "tplot2dcase.h"

tPlot2DCase::tPlot2DCase(QWidget *parent) : QWidget (parent){
    vLayout = new QVBoxLayout;
    hLayout = new QHBoxLayout;
    checkBoxLog = new QCheckBox("log");
    checkBoxHold = new QCheckBox("hold");
    checkBoxLog->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    plot2D = new tPlot2D;
    vLayout->addWidget(plot2D);
    vLayout->addLayout(hLayout);
    //hLayout->setDirection(QBoxLayout::Direction::RightToLeft);
    hLayout->addSpacing(1000);
    hLayout->addWidget(checkBoxHold);
    hLayout->addWidget(checkBoxLog);
    this->setLayout(vLayout);

    connect(checkBoxLog,SIGNAL(clicked(bool)),this,SLOT(slot_log(bool)));
}


void tPlot2DCase::slot_log(bool state){
    if(state){
        plot2D->ColorMap->setDataScaleType(QCPAxis::stLogarithmic);
    }else{
        plot2D->ColorMap->setDataScaleType(QCPAxis::stLinear);
    }
    plot2D->replot();
}

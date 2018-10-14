#include "tplot2d.h"

tPlot2D::tPlot2D(QWidget *parent) : tPlot (parent){
    ColorMap = new QCPColorMap(this->xAxis,this->yAxis);
    ColorScale = new QCPColorScale(this);
    ColorScale->setType(QCPAxis::atRight);
    this->plotLayout()->addElement(0,1,ColorScale);

    ColorMap->setColorScale(ColorScale);
    ColorMap->setGradient(QCPColorGradient::gpJet);
}

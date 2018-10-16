#include "plotdialog.h"

PlotDialog::PlotDialog(QWidget *parent) : QDialog(parent){
    mainLayout = new QVBoxLayout();
    this->setLayout(mainLayout);

    plot = new tPlot();


    mainLayout->addWidget(plot);
    this->setMinimumSize(300,200);

}

void PlotDialog::setAxiesLabel(QString LabelX,QString LabelY){
    plot->xAxis->setLabel(LabelX);
    plot->yAxis->setLabel(LabelY);
}

void PlotDialog::slot_SetAxiesLabel(QString LabelX,QString LabelY){
    setAxiesLabel(LabelX,LabelY);
}

void PlotDialog::addValue(double x,double y){
    plot->addNewValue(x,y,graph);
}

void PlotDialog::slot_addValue(double x,double y){
    addValue(x,y);
}

void PlotDialog::setGraph(int lgraph){
    graph = lgraph;
}

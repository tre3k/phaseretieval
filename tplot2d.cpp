#include "tplot2d.h"

tPlot2D::tPlot2D(QWidget *parent) : tPlot (parent){
    ColorMap = new QCPColorMap(this->xAxis,this->yAxis);
    ColorScale = new QCPColorScale(this);
    ColorScale->setType(QCPAxis::atRight);
    this->plotLayout()->addElement(0,1,ColorScale);

    ColorMap->setColorScale(ColorScale);
    ColorMap->setGradient(QCPColorGradient::gpJet);
}

void tPlot2D::plot2DMap(double **data,int size_x,int size_y){
    this->clearGraphs();
    this->ColorMap->data()->setRange(QCPRange(0,size_x),QCPRange(0,size_y));
    this->ColorMap->data()->setSize(size_x,size_y);

    for(int i=0;i<size_x;i++){
        for(int j=0;j<size_y;j++){
            this->ColorMap->data()->setCell(i,j,data[i][j]);
        }
    }

    this->ColorMap->rescaleDataRange(true);
    this->rescaleAxes();
    this->replot();
    emit signal_ready();
}

void tPlot2D::plotComplex2DMap(tComplex2D data, int param){
    this->clearGraphs();
    this->ColorMap->data()->setRange(QCPRange(0,data.getSizeX()),QCPRange(0,data.getSizeY()));
    this->ColorMap->data()->setSize(data.getSizeX(),data.getSizeY());

    for(int i=0;i<data.getSizeX();i++){
        for(int j=0;j<data.getSizeY();j++){
            switch(param){
            case PLOT2D_COMPLEX2D_AMPLITUDE:
                this->ColorMap->data()->setCell(i,j,data.data[i][j].getAmpl());
                break;

            case PLOT2D_COMPLEX2D_PHASE:
                this->ColorMap->data()->setCell(i,j,data.data[i][j].getPhase());
                break;

            case PLOT2D_COMPLEX2D_REAL:
                this->ColorMap->data()->setCell(i,j,data.data[i][j].getReal());
                break;

            case PLOT2D_COMPLEX2D_IMGN:
                this->ColorMap->data()->setCell(i,j,data.data[i][j].getImgn());
                break;
            }
        }
    }
    this->ColorMap->rescaleDataRange(true);
    this->rescaleAxes();
    this->replot();
    emit signal_ready();
}

void tPlot2D::plotComplex2DMapWithSort(tComplex2D data, int param){
    double tmp_real = 0.0;
    double tmp_imgn = 0.0;
    tComplex2D tmp_data;
    tmp_data.clone(data);

    for(int i=0;i<tmp_data.getSizeX();i++){
        for(int j=0;j<tmp_data.getSizeY()/2;j++){
            tmp_real = tmp_data.data[i][j].getReal();
            tmp_imgn = tmp_data.data[i][j].getImgn();

            tmp_data.data[i][j].setReal(tmp_data.data[i][tmp_data.getSizeY()/2+j].getReal());
            tmp_data.data[i][j].setImgn(tmp_data.data[i][tmp_data.getSizeY()/2+j].getImgn());

            tmp_data.data[i][tmp_data.getSizeY()/2+j].setReal(tmp_real);
            tmp_data.data[i][tmp_data.getSizeY()/2+j].setImgn(tmp_imgn);
        }
    }

    for(int i=0;i<tmp_data.getSizeX()/2;i++){
        for(int j=0;j<tmp_data.getSizeY();j++){
            tmp_real = tmp_data.data[i][j].getReal();
            tmp_imgn = tmp_data.data[i][j].getImgn();

            tmp_data.data[i][j].setReal(tmp_data.data[tmp_data.getSizeY()/2+i][j].getReal());
            tmp_data.data[i][j].setImgn(tmp_data.data[tmp_data.getSizeY()/2+i][j].getImgn());

            tmp_data.data[tmp_data.getSizeY()/2+i][j].setReal(tmp_real);
            tmp_data.data[tmp_data.getSizeY()/2+i][j].setImgn(tmp_imgn);
        }
    }

    plotComplex2DMap(tmp_data,param);
    tmp_data.deleteData();

    emit signal_ready();
}

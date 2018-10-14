#include "tcomplex2d.h"


tComplex2D::tComplex2D(int sx,int sy){
    SizeX = sx;
    SizeY = sy;

    data = new tComplex * [sx];
    for(int i=0;i<sx;i++) data[i] = new tComplex[sy];

}

void tComplex2D::reInit(int sx,int sy){
    for(int i=0;i<SizeX;i++) delete [] data[i];
    delete [] data;

    SizeX = sx;
    SizeY = sy;
    data = new tComplex * [sx];
    for(int i=0;i<sx;i++) data[i] = new tComplex[sy];

}

void tComplex2D::deleteData(){
    for(int i=0;i<SizeX;i++) delete [] data[i];
    delete [] data;
    return;
}

int tComplex2D::getSizeX(){
    return SizeX;
}

int tComplex2D::getSizeY(){
    return SizeY;
}

void tComplex2D::clone(tComplex2D value){
    this->reInit(value.getSizeX(),value.getSizeY());
    for(int i=0;i<value.getSizeX();i++){
        for(int j=0;j<value.getSizeY();j++){
            this->data[i][j].setReal(value.data[i][j].getReal());
            this->data[i][j].setImgn(value.data[i][j].getImgn());
        }
    }
}

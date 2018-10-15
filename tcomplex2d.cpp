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

void tComplex2D::cleanImgn(){
    for(int i=0;i<SizeX;i++){
        for(int j=0;j<SizeY;j++){
            this->data[i][j].setImgn(0.0);
        }
    }
    return;
}

void tComplex2D::reSort(){
    double tmp_real = 0.0;
    double tmp_imgn = 0.0;

    for(int i=0;i<this->getSizeX();i++){
        for(int j=0;j<this->getSizeY()/2;j++){
            tmp_real = this->data[i][j].getReal();
            tmp_imgn = this->data[i][j].getImgn();

            this->data[i][j].setReal(this->data[i][this->getSizeY()/2+j].getReal());
            this->data[i][j].setImgn(this->data[i][this->getSizeY()/2+j].getImgn());

            this->data[i][this->getSizeY()/2+j].setReal(tmp_real);
            this->data[i][this->getSizeY()/2+j].setImgn(tmp_imgn);
        }
    }

    for(int i=0;i<this->getSizeX()/2;i++){
        for(int j=0;j<this->getSizeY();j++){
            tmp_real = this->data[i][j].getReal();
            tmp_imgn = this->data[i][j].getImgn();

            this->data[i][j].setReal(this->data[this->getSizeY()/2+i][j].getReal());
            this->data[i][j].setImgn(this->data[this->getSizeY()/2+i][j].getImgn());

            this->data[this->getSizeY()/2+i][j].setReal(tmp_real);
            this->data[this->getSizeY()/2+i][j].setImgn(tmp_imgn);
        }
    }
}

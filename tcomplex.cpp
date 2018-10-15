#include "tcomplex.h"

tComplex::tComplex(){
    real = 0.0;
    imgn = real;
}

void tComplex::setReal(double value){
    real = value;
    return;
}

void tComplex::setImgn(double value){
    imgn = value;
    return;
}

void tComplex::setAmpl(double value){
    double Phase = this->getPhase();
    real = value*cos(Phase);
    imgn = value*sin(Phase);
    return;
}

void tComplex::setPhase(double value){
    double Ampl = this->getAmpl();
    real = Ampl*cos(value);
    imgn = Ampl*sin(value);
    return;
}

void tComplex::setAmplAndPhse(double Ampl,double Phase){
    real = Ampl*cos(Phase);
    imgn = Ampl*sin(Phase);
    return;
}

double tComplex::getReal(){
    return real;
}

double tComplex::getImgn(){
    return imgn;
}

double tComplex::getAmpl(){
    return sqrt(real*real + imgn*imgn);
}

double tComplex::getPhase(){
    double dphi = 0.0;
    if(real < 0 && imgn > 0) dphi = M_PI;
    if(real < 0 && imgn < 0) dphi = -M_PI;

    if(real != 0.0){
        return atan(imgn/real) + dphi;
    }else{
        return M_PI/2 + dphi;
    }
}


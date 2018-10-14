#ifndef TCOMPLEX_H
#define TCOMPLEX_H

#include <math.h>

class tComplex{
public:
    explicit tComplex();
    void setReal(double);
    void setImgn(double);
    void setAmpl(double);
    void setPhase(double);
    void setAmplAndPhse(double Ampl,double Phase);

    double getReal(void);
    double getImgn(void);
    double getAmpl(void);
    double getPhase(void);

private:
    double real;
    double imgn;
};

#endif

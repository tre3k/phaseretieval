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

    double getReal(void);
    double getImgn(void);
    double getAmpl(void);
    double getPhase(void);

private:
    double real;
    double imgn;
};

#endif

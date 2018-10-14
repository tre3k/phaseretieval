/* just my realesation the fast furje transfrm and 2D fast furje transform */

#ifndef TFFT_H
#define TFFT_H

#include <math.h>
#include "tcomplex2d.h"

class tFFT{
public:
    explicit tFFT(void);

    void simplefft(double *real,double *imgn,int n);
    void fft2d(tComplex2D *input,tComplex2D *output,double conjugation);
    void dfft2d(tComplex2D *input,tComplex2D *output);
    void ifft2d(tComplex2D *input,tComplex2D *output);
};

#endif

#ifndef TCOMPLEX2D_H
#define TCOMPLEX2D_H

#include "tcomplex.h"

class tComplex2D{
public:
    explicit tComplex2D(int sx=0,int sy=0);
    void reInit(int sx,int sy);
    int getSizeX(void);
    int getSizeY(void);
    void clone(tComplex2D);
    void cleanImgn();
    void deleteData(void);

    void reSort(void);

    tComplex **data;
private:
    int SizeX,SizeY;

};

#endif

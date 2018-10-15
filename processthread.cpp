#include "processthread.h"

ProcessThread::ProcessThread(QObject *parent) : QThread(parent){
    dp = new s_data_process;
}

void ProcessThread::run(){

    tComplex2D P(dp->inputData->getSizeX(),dp->inputData->getSizeY());

    //generateProbeSquare(&P,0.5,0.5,0.5,0.5);
    //generateProbeSquare(&P,0.5,0.5,0.24,0.24);
    //generateProbeCircle(&P,0.28,0.3,0.3);
    //generateProbeSquare(&P,0.390625,0.390625,0.390625,0.390625);
    //error_reduction(dp->inputData,&P,dp->n_itteration);
    hybrid_input_output(dp->inputData,&P,dp->betta,dp->n_itteration);
}


tComplex2D ProcessThread::error_reduction(tComplex2D *F,tComplex2D *P,int n_itteration){
    tComplex2D x,xest,X(F->getSizeX(),F->getSizeY());
    tFFT fft;
    double treshold = 10.0;

    for(int i=0;i<X.getSizeX();i++){
        for(int j=0;j<X.getSizeY();j++){
            X.data[i][j].setAmplAndPhse(F->data[i][j].getAmpl(),2*M_PI*(rand()%100-50)/100);
        }
    }

    fft.ifft2d(&X,&x);
    x.cleanImgn();

    for(int k=0;k<n_itteration;k++){
        // direct FFT
        fft.dfft2d(&x,&X);

        if((k+1)%50==0){
            emit signal_plotResult(&x);
            emit signal_plotAmpl(&X);
            emit signal_plotPhase(&X);
            qDebug() << "Err: " << findError(F,&X);
            this->msleep(500);
        }

        // replace Amplitude
        for(int i=0;i<X.getSizeX();i++){
            for(int j=0;j<X.getSizeY();j++){
                X.data[i][j].setAmpl(F->data[i][j].getAmpl());
            }
        }

        // inverse FFT
        fft.ifft2d(&X,&xest);
        xest.cleanImgn();          // keepout only the real space

        // constrains

        for(int i=0;i<xest.getSizeX();i++){
            for(int j=0;j<xest.getSizeY();j++){
                if((xest.data[i][j].getReal() > 0) && (P->data[i][j].getReal() > 0.5)){
                    x.data[i][j].setReal(xest.data[i][j].getReal());
                }else{
                    x.data[i][j].setReal(0.0);
                }
            }
        }


        for(int i=0;i<xest.getSizeX();i++){
            for(int j=0;j<xest.getSizeY();j++){
                if(xest.data[i][j].getReal() > treshold){
                    x.data[i][j].setReal(treshold);
                }
            }
        }

        emit signal_setProgress((k+1)*100/n_itteration);
        emit signal_showMessage("Itteration: "+QString::number(k+1));
    }


    emit signal_plotResult(&x);
    emit signal_plotAmpl(&X);
    emit signal_plotPhase(&X);

    this->sleep(1);

    return x;
}

tComplex2D ProcessThread::hybrid_input_output(tComplex2D *F,tComplex2D *P,double betta,int n_itteration){
    tComplex2D x,xest,X(F->getSizeX(),F->getSizeY());
    tFFT fft;
    double treshold = 19.0;  //for SANS
    //double treshold = 1.0;

    for(int i=0;i<X.getSizeX();i++){
        for(int j=0;j<X.getSizeY();j++){
            X.data[i][j].setAmplAndPhse(F->data[i][j].getAmpl(),2*M_PI*(rand()%100-50)/100);
        }
    }

    fft.ifft2d(&X,&x);
    x.cleanImgn();

    for(int k=0;k<n_itteration;k++){
        // direct FFT
        fft.dfft2d(&x,&X);

        if((k+1)%50==0){
            emit signal_plotResult(&x);
            emit signal_plotAmpl(&X);
            emit signal_plotPhase(&X);
            qDebug() << "Err: " << findError(F,&X);
            this->msleep(500);
        }

        // replace Amplitude
        for(int i=0;i<X.getSizeX();i++){
            for(int j=0;j<X.getSizeY();j++){
                X.data[i][j].setAmpl(F->data[i][j].getAmpl());
            }
        }

        // inverse FFT
        fft.ifft2d(&X,&xest);
        xest.cleanImgn();          // keepout only the real space

        // constrains

        for(int i=0;i<xest.getSizeX();i++){
            for(int j=0;j<xest.getSizeY();j++){
                if((xest.data[i][j].getReal() > 0) && (P->data[i][j].getReal() > 0.5)){
                    x.data[i][j].setReal(xest.data[i][j].getReal());
                }else{
                    x.data[i][j].setReal(x.data[i][j].getReal() - betta*xest.data[i][j].getReal());
                }
            }
        }


        for(int i=0;i<xest.getSizeX();i++){
            for(int j=0;j<xest.getSizeY();j++){
                if(xest.data[i][j].getReal() > treshold){
                    x.data[i][j].setReal(treshold);
                }
            }
        }

        emit signal_setProgress((k+1)*100/n_itteration);
        emit signal_showMessage("Itteration: "+QString::number(k+1));
    }


    emit signal_plotResult(&x);
    emit signal_plotAmpl(&X);
    emit signal_plotPhase(&X);

    this->sleep(1);

    return x;
}

double ProcessThread::findError(tComplex2D *ideal,tComplex2D *compare){
    double S1,S2,Err;
    S1 = 0.0;
    S2 = S1;

    for(int i=0;i<ideal->getSizeX();i++){
        for(int j=0;j<ideal->getSizeY();j++){
            S1 += (compare->data[i][j].getAmpl()-ideal->data[i][j].getAmpl())*
                  (compare->data[i][j].getAmpl()-ideal->data[i][j].getAmpl());
            S2 += ideal->data[i][j].getAmpl()*ideal->data[i][j].getAmpl();
        }
    }

    Err = sqrt(S1/S2);

    return Err;
}

void ProcessThread::generateProbeSquare(tComplex2D *data,double center_x,double center_y,double w,double h){
    int i,j;
    int minSize = data->getSizeX();
    if(minSize > data->getSizeY()) minSize = data->getSizeY();

    center_x *= minSize;
    center_y *= minSize;
    w *= minSize;
    h *= minSize;

    for(i=doubleToInt(center_x-w);i<doubleToInt(center_x+w);i++){
        for(j=doubleToInt(center_y-h);j<doubleToInt(center_y+h);j++){
            if(i<0 || j<0 || i>=data->getSizeX() || j>=data->getSizeY()) continue;
            data->data[i][j].setReal(1.0);
        }
    }

    return;
}

void ProcessThread::generateProbeCircle(tComplex2D *data,double r,double x,double y){
    int i=0,j=0;
    int minSize = data->getSizeX();
    if(minSize > data->getSizeY()) minSize = data->getSizeY();

    r *= minSize;
    x *= minSize;
    y *= minSize;

    for(i=doubleToInt(x-r);i<doubleToInt(x+r);i++){
        for(j=doubleToInt(y-r);j<doubleToInt(y+r);j++){
            if(i<0 || j<0 || i>=data->getSizeX() || j>=data->getSizeY()) continue;
            if((i-x)*(i-x)+(j-y)*(j-y) < r*r) data->data[i][j].setReal(1.0);
        }
    }
    return;
}

int ProcessThread::doubleToInt(double value){
    return (int)(value+0.5);
}

void ProcessThread::slot_recive_data_process(s_data_process *lpd){
    dp = lpd;
    return;
}


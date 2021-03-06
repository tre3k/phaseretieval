#include "processthread.h"

ProcessThread::ProcessThread(QObject *parent) : QThread(parent){
    dp = new s_data_process;
}

void ProcessThread::run(){

    tComplex2D P(dp->inputData->getSizeX(),dp->inputData->getSizeY());

    //generateProbeSquare(&P,0.390625,0.390625,0.390625,0.390625);

    switch (dp->probe){
    case PROBE_FUNCTION_SQUARE:
        generateProbeSquare(&P,dp->probeParam1,dp->probeParam2,
                               dp->probeParam4,dp->probeParam3);
        break;
    case PROBE_FUNCTION_CIRCLE:
        generateProbeCircle(&P,dp->probeParam3,dp->probeParam1,dp->probeParam2);
        break;
    }

    switch(dp->method){
    case METHOD_ERROR_REDUCTION:
        error_reduction(dp->inputData,&P,dp->n_itteration);
        break;
    case METHOD_HYBRID_IN_OUT:
        hybrid_input_output(dp->inputData,&P,dp->betta,dp->n_itteration);
        break;
    }
}


tComplex2D ProcessThread::error_reduction(tComplex2D *F,tComplex2D *P,int n_itteration){
    tComplex2D x,xest,X(F->getSizeX(),F->getSizeY());
    tFFT fft;
    double treshold = dp->treshold;

    for(int i=0;i<X.getSizeX();i++){
        for(int j=0;j<X.getSizeY();j++){
            X.data[i][j].setAmplAndPhse(F->data[i][j].getAmpl(),2*M_PI*(rand()%100-50)/100);
        }
    }

    fft.ifft2d(&X,&x);
    x.cleanImgn();

    for(int k=0;k<n_itteration;k++){
        // direct FFT
        startTime = QDateTime::currentDateTime();
        fft.dfft2d(&x,&X);

        if((k+1)%dp->n_of_plots==0) plotGraphs(&x,&X,F,k);

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
                if((xest.data[i][j].getReal() > dp->background) && (P->data[i][j].getReal() > 0.5)){
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

        endTime = QDateTime::currentDateTime();
        mstime = startTime.time().msecsTo(endTime.time());

        emit signal_setProgress((k+1)*100/n_itteration);
        emit signal_showMessage("Itteration: "+QString::number(k+1)+" Remaining time: "
                                +QString::number(1.0*mstime*(n_itteration-k)/1000.0)+" s.");
        ready = false;
    }

    emit signal_plotResult(&x,n_itteration-1);
    emit signal_plotAmpl(&X);
    emit signal_plotPhase(&X,n_itteration-1);

    this->sleep(1);

    return x;
}

tComplex2D ProcessThread::hybrid_input_output(tComplex2D *F,tComplex2D *P,double betta,int n_itteration){
    tComplex2D x,xest,X(F->getSizeX(),F->getSizeY());
    tFFT fft;
    //double treshold = 19.0;  //for SANS
    double treshold = dp->treshold;

    for(int i=0;i<X.getSizeX();i++){
        for(int j=0;j<X.getSizeY();j++){
            X.data[i][j].setAmplAndPhse(F->data[i][j].getAmpl(),2*M_PI*(rand()%100-50)/100);
        }
    }

    fft.ifft2d(&X,&x);
    x.cleanImgn();

    for(int k=0;k<n_itteration;k++){
        startTime = QDateTime::currentDateTime();
        // direct FFT
        fft.dfft2d(&x,&X);

        if((k+1)%dp->n_of_plots==0) plotGraphs(&x,&X,F,k);

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
                if((xest.data[i][j].getReal() > dp->background) && (P->data[i][j].getReal() > 0.5)){
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


        endTime = QDateTime::currentDateTime();
        mstime = startTime.time().msecsTo(endTime.time());

        emit signal_setProgress((k+1)*100/n_itteration);
        emit signal_showMessage("Itteration: "+QString::number(k+1)+" Remaining time: "
                                +QString::number(1.0*mstime*(n_itteration-k)/1000.0)+" s.");
    }


    emit signal_plotResult(&x,n_itteration-1);
    emit signal_plotAmpl(&X);
    emit signal_plotPhase(&X,n_itteration-1);

    this->sleep(1);

    return x;
}

double ProcessThread::findError(tComplex2D *ideal,tComplex2D *compare){
    double S1,S2,Err;
    S1 = 0.0;
    S2 = S1;
    double max = ideal->data[0][0].getReal();

    for(int i=0;i<ideal->getSizeX();i++){
        for(int j=0;j<ideal->getSizeY();j++){
            S1 += (compare->data[i][j].getAmpl()-ideal->data[i][j].getAmpl())*
                  (compare->data[i][j].getAmpl()-ideal->data[i][j].getAmpl());
            S2 += ideal->data[i][j].getAmpl()*ideal->data[i][j].getAmpl();
            if(ideal->data[i][j].getReal() > max) max = ideal->data[i][j].getReal();
        }
    }

    Err = sqrt(S1/S2)/max;

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

void ProcessThread::slot_ready(){
    ready = true;
}

void ProcessThread::plotGraphs(tComplex2D *x,tComplex2D *X, tComplex2D *F, int i){
    emit signal_plotResult(x, i);
    emit signal_plotAmpl(X);
    emit signal_plotPhase(X,i);
    emit signal_plotErrorValue(i,findError(F,X));
    this->msleep(100);
    while(!ready);
}

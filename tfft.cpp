#include "tfft.h"

tFFT::tFFT(){

}

void tFFT::simplefft(double *real,double *imgn,int n){
    int step,start,old_start,i,j,theta=0;
    double tmpReal,tmpImgn;

    /******** Reverse bits *****/
    int b=0,l,k,bits=log(n)/log(2);
    double *copy_real = new double [n];
    double *copy_imgn = new double [n];
    for(i=0;i<n;i++) copy_real[i]=real[i];
    for(i=0;i<n;i++) copy_imgn[i]=imgn[i];
    for(i=0;i<n;i++){
        b=0,k=bits;
        for(l=0;l<bits;l++){
            k--;
            b|=(((i&(1<<l)))>>l)<<k;
        }

        real[i] = copy_real[b];
        imgn[i] = copy_imgn[b];
    }
    delete [] copy_imgn;
    delete [] copy_real;

    /****************************/

    for(step=1;step<n;step = step << 1){
        old_start=0;
        for(start=step << 1;start<=n;start+=step << 1){
            theta = 0;
            for(i=old_start;i<start-step;i++){
                j=i+step;
                /* a-bc */

                tmpReal = real[i]-(real[j]*cos(-theta*2*M_PI/(step<<1))-
                                   imgn[j]*sin(-theta*2*M_PI/(step<<1)));
                tmpImgn = imgn[i]-(real[j]*sin(-theta*2*M_PI/(step<<1))+
                                   imgn[j]*cos(-theta*2*M_PI/(step<<1)));

                /* a+bc */

                real[i] = real[i]+(real[j]*cos(-theta*2*M_PI/(step<<1))-
                                   imgn[j]*sin(-theta*2*M_PI/(step<<1)));
                imgn[i] = imgn[i]+(real[j]*sin(-theta*2*M_PI/(step<<1))+
                                   imgn[j]*cos(-theta*2*M_PI/(step<<1)));

                real[j] = tmpReal;
                imgn[j] = tmpImgn;

                theta ++;

            }
            old_start = start;
        }
    }
}


/* Fast furje transform */
void tFFT::fft2d(tComplex2D *input,tComplex2D *output, double conjugation){
    int N = input->getSizeX();
    int M = input->getSizeY();

    output->reInit(input->getSizeX(),input->getSizeY());


    double *tmp_real;
    double *tmp_imgn;

    tmp_real = new double [N];
    tmp_imgn = new double [N];

    for(int j=0;j<M;j++){
        for(int i=0;i<N;i++){
            tmp_real[i] = input->data[i][j].getReal();
            tmp_imgn[i] = conjugation*input->data[i][j].getImgn();
        }
        simplefft(tmp_real,tmp_imgn,N);
        for(int i=0;i<N;i++){
            output->data[i][j].setReal(tmp_real[i]);
            output->data[i][j].setImgn(tmp_imgn[i]);
        }
    }

    delete [] tmp_real;
    delete [] tmp_imgn;


    tmp_real = new double [M];
    tmp_imgn = new double [M];

    for(int i=0;i<N;i++){
        for(int j=0;j<M;j++){
            tmp_real[j] = output->data[i][j].getReal();
            tmp_imgn[j] = output->data[i][j].getImgn();
        }
        simplefft(tmp_real,tmp_imgn,M);
        for(int j=0;j<M;j++){
            output->data[i][j].setReal(tmp_real[j]);
            output->data[i][j].setImgn(tmp_imgn[j]);
        }
    }

    delete [] tmp_real;
    delete [] tmp_imgn;

    return;
}

/* direct transform */
void tFFT::dfft2d(tComplex2D *input,tComplex2D *output){
    fft2d(input,output,1.0);
}

/* inverse transform */
void tFFT::ifft2d(tComplex2D *input,tComplex2D *output){
    fft2d(input,output,-1.0);
}



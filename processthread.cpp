#include "processthread.h"

ProcessThread::ProcessThread(QObject *parent) : QThread(parent){
    dp = new s_data_process;
}

void ProcessThread::run(){


}

void ProcessThread::slot_recive_data_process(s_data_process *lpd){
    dp = lpd;
    return;
}


tComplex2D ProcessThread::error_reduction(tComplex2D F,int n_itteration){
    tComplex2D x,xest,X;
    X.clone(F);



    return x;
}

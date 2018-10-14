#ifndef PROCESSTHREAD_H
#define PROCESSTHREAD_H

#include <QThread>

class ProcessThread : public QThread{
    Q_OBJECT
public:
    explicit ProcessThread(QObject *parent = nullptr);
    void run();

};


#endif

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QGridLayout *mainLayout = new QGridLayout();
    QWidget *mainWidget = new QWidget();

    process = new ProcessThread();

    plotIn = new tPlot2DCase();
    plotOut = new tPlot2DCase(); ;
    plotInAmpl = new tPlot2DCase();
    plotInPhase = new tPlot2DCase();
    plotOutAmpl = new tPlot2DCase();
    plotOutPhase = new tPlot2DCase();

    plotIn->setMinimumSize(20,20);
    plotInAmpl->setMinimumSize(20,20);
    plotInPhase->setMinimumSize(20,20);
    plotOut->setMinimumSize(20,20);
    plotOutAmpl->setMinimumSize(20,20);
    plotOutPhase->setMinimumSize(20,20);

    plotInPhase->plot2D->ColorScale->axis()->setTicker(QSharedPointer<QCPAxisTickerPi>(new QCPAxisTickerPi));
    plotOutPhase->plot2D->ColorScale->axis()->setTicker(QSharedPointer<QCPAxisTickerPi>(new QCPAxisTickerPi));

    plotIn->plot2D->ColorMap->setGradient(QCPColorGradient::gpGrayscale);
    plotOut->plot2D->ColorMap->setGradient(QCPColorGradient::gpGrayscale);

    mainLayout->addWidget(plotIn,0,0);
    mainLayout->addWidget(plotInAmpl,0,1);
    mainLayout->addWidget(plotInPhase,0,2);
    mainLayout->addWidget(plotOut,1,0);
    mainLayout->addWidget(plotOutAmpl,1,1);
    mainLayout->addWidget(plotOutPhase,1,2);


    mainWidget->setLayout(mainLayout);
    this->setCentralWidget(mainWidget);

    progressBar = new QProgressBar;
    progressBar->setMaximumSize(100,13);
    progressBar->setMaximum(100);

    this->statusBar()->addPermanentWidget(progressBar);


    connect(this,SIGNAL(signal_send_data_process(s_data_process *)),
            process,SLOT(slot_recive_data_process(s_data_process *)));
    connect(process,SIGNAL(signal_showMessage(QString)),
            this,SLOT(slot_showMessage(QString)));

    connect(process,SIGNAL(signal_setProgress(int)),
            this,SLOT(slot_setProgress(int)));

    connect(process,SIGNAL(signal_plotResult(tComplex2D *)),
            this,SLOT(slot_plotResult(tComplex2D *)));
    connect(process,SIGNAL(signal_plotAmpl(tComplex2D *)),
            this,SLOT(slot_plotOutAmpl(tComplex2D *)));
    connect(process,SIGNAL(signal_plotPhase(tComplex2D *)),
            this,SLOT(slot_plotOutPhase(tComplex2D *)));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionExit_triggered()
{
    QApplication::quit();
}

void MainWindow::on_actionProcess_triggered()
{


    s_data_process *data_process = new s_data_process;
    data_process->inputData = &measure;
    data_process->n_itteration = 10000;
    data_process->betta = 0.84;


    emit signal_send_data_process(data_process);
    process->start();

}

void MainWindow::on_actionOpen_image_triggered()
{
    QString filename = QFileDialog::getOpenFileName(this,"Image","","All types (*.jpg *.jpeg *.JPG *.JPEG *.bmp *.BMP *.gif "
                                                                    "*.GIF *.png *.PNG *.pbm *.PBM *.pgm *.PGM *.ppm *.PPM *.xbm *.XBM *.xpm *.XPM);;"
                                                                    "Jpeg (*.jpg *.jpeg *.JPG *.JPEG);;"
                                                                    "BMP (*.bmp *.BMP);;"
                                                                    "GIF (*.gif *.GIF);;"
                                                                    "PNG (*.png *.PNG);;"
                                                                    "PBM (*.pbm *.PBM);;"
                                                                    "PGM (*.pgm *.PGM);;"
                                                                    "PPM (*.ppm *.PPM);;"
                                                                    "XBM (*.xbm *.XBM);;"
                                                                    "XPM (*.xpm *.XPM)");

    if(filename=="") return;
    OpenImage(filename, &InputImage);
    plotIn->plot2D->plotComplex2DMap(InputImage,PLOT2D_COMPLEX2D_REAL);

    tFFT fft;
    fft.dfft2d(&InputImage,&measure);

    plotInAmpl->plot2D->plotComplex2DMapWithSort(measure,PLOT2D_COMPLEX2D_AMPLITUDE);
    plotInPhase->plot2D->plotComplex2DMapWithSort(measure,PLOT2D_COMPLEX2D_PHASE);


    for(int i=0;i<measure.getSizeX();i++){
        for(int j=0;j<measure.getSizeY();j++){
            measure.data[i][j].setPhase(0.0);
        }
    }
}

void MainWindow::OpenImage(QString filename, tComplex2D *output){
    int sx,sy,tmp;

    double nx,ny;

    QImage *img = new QImage(filename);

    sx = img->width();
    sy = img->height();

    nx = log(sx)/log(2);
    ny = log(sy)/log(2);

    if((nx!=int(nx))){
        sx = pow(2,int(nx)+1);
    }
    if((ny!=int(ny))){
        sy = pow(2,int(ny)+1);
    }

    output->reInit(sx,sy);

    for(int i=0;i<img->width();i++){
        for(int j=0;j<img->height();j++){
            tmp =   img->pixel(i,j) & 0x00000000ff;                  //B
            tmp += (img->pixel(i,j) & 0x000000ff00)>>8;              //G
            tmp += (img->pixel(i,j) & 0x0000ff0000)>>16;             //R

            output->data[i][img->height()-j-1].setReal((double)(tmp)/765.0);
        }
    }


    return;
}

void MainWindow::slot_showMessage(QString msg){
    this->statusBar()->showMessage(msg);
}

void MainWindow::slot_setProgress(int value){
    progressBar->setValue(value);
}


void MainWindow::slot_plotResult(tComplex2D *data){
    plotOut->plot2D->plotComplex2DMap(*data,PLOT2D_COMPLEX2D_REAL);
    counter ++;

    //plotOut->plot2D->saveBmp("/tmp/out_"+QString::number(counter)+".bmp");

    return;
}

void MainWindow::slot_plotOutAmpl(tComplex2D *data){
    plotOutAmpl->plot2D->plotComplex2DMapWithSort(*data,PLOT2D_COMPLEX2D_AMPLITUDE);
    return;
}

void MainWindow::slot_plotOutPhase(tComplex2D *data){
    plotOutPhase->plot2D->plotComplex2DMapWithSort(*data,PLOT2D_COMPLEX2D_PHASE);
    return;
}

void MainWindow::on_actionOpen_Amplitude_triggered()
{
    QString filename = QFileDialog::getOpenFileName(this,"Image","","All types (*.jpg *.jpeg *.JPG *.JPEG *.bmp *.BMP *.gif "
                                                                    "*.GIF *.png *.PNG *.pbm *.PBM *.pgm *.PGM *.ppm *.PPM *.xbm *.XBM *.xpm *.XPM);;"
                                                                    "Jpeg (*.jpg *.jpeg *.JPG *.JPEG);;"
                                                                    "BMP (*.bmp *.BMP);;"
                                                                    "GIF (*.gif *.GIF);;"
                                                                    "PNG (*.png *.PNG);;"
                                                                    "PBM (*.pbm *.PBM);;"
                                                                    "PGM (*.pgm *.PGM);;"
                                                                    "PPM (*.ppm *.PPM);;"
                                                                    "XBM (*.xbm *.XBM);;"
                                                                    "XPM (*.xpm *.XPM)");

    if(filename=="") return;
    OpenImage(filename, &measure);
    measure.reSort();
    for(int i=0;i<measure.getSizeX();i++){
        for(int j=0;j<measure.getSizeY();j++){
            measure.data[i][j].setReal(measure.data[i][j].getReal());
        }
    }
    plotInAmpl->plot2D->plotComplex2DMapWithSort(measure,PLOT2D_COMPLEX2D_AMPLITUDE);
}

void MainWindow::ReadSANS1(tComplex2D *data, QString filename){
    double maxValue = 0;
    enum{
            File,
            Sample,
            Setup,
            Counter,
            History,
            Comment,
            Counts
        };
        int segment;
        int x=0,y=0;
        int sx,sy;
        bool readData = false;

        QFile f(filename);
        f.open(QIODevice::ReadOnly);
        QTextStream stream(&f);

        QString tmp;
        QString value;
        QStringList stringData;

        while(!stream.atEnd()){
                tmp = stream.readLine();

                if(tmp=="%File") segment=File;
                if(tmp=="%Sample") segment=Sample;
                if(tmp=="%Setup"){
                    segment=Setup;
                    data->reInit(sx,sy);
                }
                if(tmp=="%Counter") segment=Counter;
                if(tmp=="%History") segment=History;
                if(tmp=="%Comment") segment=Comment;
                if(tmp=="%Counts")  readData = true;

                switch(segment){
                case File:
                    if(tmp.split('=').at(0)=="DataSizeX"){
                        value=tmp.split('=').at(1);
                        sx=value.toInt();
                    }
                    if(tmp.split('=').at(0)=="DataSizeY"){
                        value=tmp.split('=').at(1);
                        sy=value.toInt();
                    }
                    break;

                }

                if(readData){
                    y++;
                    stringData = tmp.split(',');
                    for(x=0;x<stringData.size();x++){
                        if(QString(stringData.at(x)).toDouble() > maxValue) maxValue = QString(stringData.at(x)).toDouble();
                        data->data[x][y-1].setReal(QString(stringData.at(x)).toDouble());
                    }
                }

        }

        for(int i=0;i<data->getSizeX();i++){
            for(int j=0;j<data->getSizeY();j++){
                data->data[i][j].setReal(sqrt(data->data[i][j].getReal()/maxValue));
            }
        }

}


void MainWindow::on_actionOpen_SANS_1_data_triggered()
{
    QString filename = QFileDialog::getOpenFileName(this,"SANS","","SANS-1 (*.001)");
    if(filename=="") return;
    ReadSANS1(&measure,filename);
    measure.reSort();
    plotInAmpl->plot2D->plotComplex2DMapWithSort(measure,PLOT2D_COMPLEX2D_AMPLITUDE);
}

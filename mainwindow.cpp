#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    Options = new s_options;
    /* default options */
    Options->number_of_plots = 20;
    Options->image_format = IMAGE_FORMAT_JPG;
    Options->save_directory = "./out/";


    QGridLayout *mainLayout = new QGridLayout();
    QWidget *mainWidget = new QWidget();

    /* create QThread process */
    process = new ProcessThread();

    /* build plots */
    plotIn = new tPlot2DCase();
    plotOut = new tPlot2DCase(); ;
    plotInAmpl = new tPlot2DCase();
    plotInPhase = new tPlot2DCase();
    plotOutAmpl = new tPlot2DCase();
    plotOutPhase = new tPlot2DCase();

    plotInAmpl->checkBoxLog->setChecked(true);
    plotInAmpl->slot_log(true);
    plotOutAmpl->checkBoxLog->setChecked(true);
    plotOutAmpl->slot_log(true);

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

    /* create SpinBox for parameters of methods*/
    spinBoxItteration = new QSpinBox();
    spinBoxBetta = new QDoubleSpinBox();
    spinBoxTreshold = new QDoubleSpinBox();
    spinBoxBackground = new QDoubleSpinBox();

    spinBoxItteration->setMaximum(999999);
    spinBoxItteration->setValue(800);
    spinBoxTreshold->setValue(1.0);
    spinBoxTreshold->setSingleStep(0.1);
    spinBoxTreshold->setMaximum(999999);
    spinBoxTreshold->setMinimum(0);
    spinBoxBackground->setValue(0.0);
    spinBoxBackground->setSingleStep(0.1);
    spinBoxBackground->setMaximum(999999);
    spinBoxBackground->setMinimum(-999999);
    spinBoxBetta->setValue(0.78);
    spinBoxBetta->setDecimals(2);
    spinBoxBetta->setSingleStep(0.01);
    spinBoxBetta->setEnabled(false);

    comboSelectMethod = new QComboBox();
    comboSelectMethod->addItem("Error reduction");
    comboSelectMethod->addItem("HIO");

    ui->mainToolBar->addSeparator();
    ui->mainToolBar->addWidget(new QLabel(" Treshold: "));
    ui->mainToolBar->addWidget(spinBoxTreshold);
    ui->mainToolBar->addWidget(new QLabel(" Background: "));
    ui->mainToolBar->addWidget(spinBoxBackground);
    ui->mainToolBar->addWidget(new QLabel(" Betta: "));
    ui->mainToolBar->addWidget(spinBoxBetta);
    ui->mainToolBar->addWidget(new QLabel(" N itteration: "));
    ui->mainToolBar->addWidget(spinBoxItteration);
    ui->mainToolBar->addSeparator();
    ui->mainToolBar->addWidget(comboSelectMethod);


    /* create second ToolBar for parameters of Probe Functions */
    secondToolBar = new QToolBar("Probe function");
    this->addToolBar(secondToolBar);

    comboSelectProbe = new QComboBox();
    comboSelectProbe->addItem("Square");
    comboSelectProbe->addItem("Circle");

    labelPorbe1 = new QLabel(" x0: ");
    labelPorbe2 = new QLabel(" y0: ");
    labelPorbe3 = new QLabel(" w: ");
    labelPorbe4 = new QLabel(" h: ");

    spinBoxProbe1 = new QDoubleSpinBox();
    spinBoxProbe1->setRange(0.0,1.0);
    spinBoxProbe1->setValue(0.5);
    spinBoxProbe1->setSingleStep(0.01);
    spinBoxProbe2 = new QDoubleSpinBox();
    spinBoxProbe2->setRange(0.0,1.0);
    spinBoxProbe2->setValue(0.5);
    spinBoxProbe2->setSingleStep(0.01);
    spinBoxProbe3 = new QDoubleSpinBox();
    spinBoxProbe3->setRange(0.0,1.0);
    spinBoxProbe3->setValue(0.5);
    spinBoxProbe3->setSingleStep(0.01);
    spinBoxProbe4 = new QDoubleSpinBox();
    spinBoxProbe4->setRange(0.0,1.0);
    spinBoxProbe4->setValue(0.5);
    spinBoxProbe4->setSingleStep(0.01);

    secondToolBar->addWidget(new QLabel("Probe function: "));
    secondToolBar->addWidget(comboSelectProbe);
    secondToolBar->addWidget(labelPorbe1);
    secondToolBar->addWidget(spinBoxProbe1);
    secondToolBar->addWidget(labelPorbe2);
    secondToolBar->addWidget(spinBoxProbe2);
    secondToolBar->addWidget(labelPorbe3);
    secondToolBar->addWidget(spinBoxProbe3);
    secondToolBar->addWidget(labelPorbe4);
    secondToolBar->addWidget(spinBoxProbe4);


    /* place plots on central widget */
    mainLayout->addWidget(plotIn,0,0);
    mainLayout->addWidget(plotInAmpl,0,1);
    mainLayout->addWidget(plotInPhase,0,2);
    mainLayout->addWidget(plotOut,1,0);
    mainLayout->addWidget(plotOutAmpl,1,1);
    mainLayout->addWidget(plotOutPhase,1,2);

    mainWidget->setLayout(mainLayout);
    this->setCentralWidget(mainWidget);

    /* and place progressBar on StatusBar */
    progressBar = new QProgressBar;
    progressBar->setMaximumSize(100,13);
    progressBar->setMaximum(100);

    this->statusBar()->addPermanentWidget(progressBar);

    dialogErrors = new PlotDialog();
    dialogErrors->setAxiesLabel("itteration","Error");
    dialogOptions = new OptionDialog();


    /* Connections signals and slots*/
    connect(this,SIGNAL(signal_send_data_process(s_data_process *)),
            process,SLOT(slot_recive_data_process(s_data_process *)));
    connect(process,SIGNAL(signal_showMessage(QString)),
            this,SLOT(slot_showMessage(QString)));

    connect(process,SIGNAL(signal_setProgress(int)),
            this,SLOT(slot_setProgress(int)));

    connect(process,SIGNAL(signal_plotResult(tComplex2D *, int)),
            this,SLOT(slot_plotResult(tComplex2D *, int)));
    connect(process,SIGNAL(signal_plotAmpl(tComplex2D *)),
            this,SLOT(slot_plotOutAmpl(tComplex2D *)));
    connect(process,SIGNAL(signal_plotPhase(tComplex2D *, int)),
            this,SLOT(slot_plotOutPhase(tComplex2D *, int)));

    connect(comboSelectMethod,SIGNAL(activated(int)),
            this,SLOT(slot_comboSelectedMethod(int)));
    connect(comboSelectProbe,SIGNAL(activated(int)),
            this,SLOT(slot_comboSelectedProbe(int)));

    connect(plotOutPhase->plot2D,SIGNAL(signal_ready()),
            process,SLOT(slot_ready()));

    connect(spinBoxProbe1,SIGNAL(valueChanged(double)),
            this,SLOT(slot_spinBoxProbeChanged(double)));
    connect(spinBoxProbe2,SIGNAL(valueChanged(double)),
            this,SLOT(slot_spinBoxProbeChanged(double)));
    connect(spinBoxProbe3,SIGNAL(valueChanged(double)),
            this,SLOT(slot_spinBoxProbeChanged(double)));
    connect(spinBoxProbe4,SIGNAL(valueChanged(double)),
            this,SLOT(slot_spinBoxProbeChanged(double)));

    connect(process,SIGNAL(signal_plotErrorValue(double,double)),
            dialogErrors,SLOT(slot_addValue(double,double)));

    connect(dialogOptions,SIGNAL(signal_sendOptionsData(s_options *)),
            this,SLOT(slot_recvOptionsData(s_options *)));

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
    data_process->n_itteration = spinBoxItteration->value();
    data_process->betta = spinBoxBetta->value();
    data_process->treshold = spinBoxTreshold->value();
    data_process->method = comboSelectMethod->currentIndex();
    data_process->background = spinBoxBackground->value();

    data_process->probeParam1 = spinBoxProbe1->value();
    data_process->probeParam2 = spinBoxProbe2->value();
    data_process->probeParam3 = spinBoxProbe3->value();
    data_process->probeParam4 = spinBoxProbe4->value();

    data_process->probe = comboSelectProbe->currentIndex();

    dialogErrors->plot->clearGraphs();
    dialogErrors->setGraph(dialogErrors->plot->createGraph());

    data_process->dialogErrors = dialogErrors;
    data_process->n_of_plots = 40;

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

#ifdef BEAM_STOP_EMULATION
    for(int i=0;i<measure.getSizeX();i++){
        for(int j=0;j<measure.getSizeY();j++){
            if(i < 6 && j < 6 ) measure.data[i][j].setAmpl(0.0);
            if(i < 6 && j > measure.getSizeY()-6 ) measure.data[i][j].setAmpl(0.0);
            if(i > measure.getSizeX()-6 && j < 6 ) measure.data[i][j].setAmpl(0.0);
            if(i > measure.getSizeX()-6 && j > measure.getSizeY()-6 ) measure.data[i][j].setAmpl(0.0);
        }
    }

    fft.ifft2d(&measure,&InputImage);
    plotOut->plot2D->plotComplex2DMap(InputImage,PLOT2D_COMPLEX2D_REAL);
#endif

    plotInAmpl->plot2D->plotComplex2DMapWithSort(measure,PLOT2D_COMPLEX2D_AMPLITUDE);
    plotInPhase->plot2D->plotComplex2DMapWithSort(measure,PLOT2D_COMPLEX2D_PHASE);


    for(int i=0;i<measure.getSizeX();i++){
        for(int j=0;j<measure.getSizeY();j++){
            measure.data[i][j].setPhase(0.0);
        }
    }

    switch (Options->image_format) {
    case IMAGE_FORMAT_JPG:
        plotIn->plot2D->saveJpg(Options->save_directory+"/Original.jpg");
        plotInAmpl->plot2D->saveJpg(Options->save_directory+"/Original_spectra.jpg");
        plotInPhase->plot2D->saveJpg(Options->save_directory+"/Original_phase.jpg");
        break;
    case IMAGE_FORMAT_PNG:
        plotIn->plot2D->savePng(Options->save_directory+"/Original.png");
        plotInAmpl->plot2D->savePng(Options->save_directory+"/Original_spectra.png");
        plotInPhase->plot2D->savePng(Options->save_directory+"/Original_phase.png");
        break;
    }


    preparePlotOut();
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


void MainWindow::slot_plotResult(tComplex2D *data, int i){
    plotOut->plot2D->plotComplex2DMap(*data,PLOT2D_COMPLEX2D_REAL);
    counter ++;


    switch(Options->image_format){
    case IMAGE_FORMAT_JPG:
        plotOut->plot2D->saveJpg(Options->save_directory+"/out_"+QString::number(i+1)+".jpg");
        break;
    case IMAGE_FORMAT_PNG:
        plotOut->plot2D->savePng(Options->save_directory+"/out_"+QString::number(i+1)+".png");
        break;
    }

    //plotOut->plot2D->saveBmp("/tmp/out_"+QString::number(counter)+".bmp");

    return;
}

void MainWindow::slot_plotOutAmpl(tComplex2D *data){
    plotOutAmpl->plot2D->plotComplex2DMapWithSort(*data,PLOT2D_COMPLEX2D_AMPLITUDE);
    return;
}

void MainWindow::slot_plotOutPhase(tComplex2D *data, int i){
    plotOutPhase->plot2D->plotComplex2DMapWithSort(*data,PLOT2D_COMPLEX2D_PHASE);

    switch(Options->image_format){
    case IMAGE_FORMAT_JPG:
        plotOutPhase->plot2D->saveJpg(Options->save_directory+"/out_phase_"+QString::number(i+1)+".jpg");
        break;
    case IMAGE_FORMAT_PNG:
        plotOutPhase->plot2D->savePng(Options->save_directory+"/out_phase_"+QString::number(i+1)+".png");
        break;
    }

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
            measure.data[i][j].setReal(sqrt(measure.data[i][j].getReal()));
        }
    }
    plotInAmpl->plot2D->plotComplex2DMapWithSort(measure,PLOT2D_COMPLEX2D_AMPLITUDE);
    preparePlotOut();
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
                //data->data[i][j].setReal(data->data[i][j].getReal()/maxValue);
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
    preparePlotOut();
}

void MainWindow::on_actionStop_triggered()
{
    process->terminate();
}

void MainWindow::slot_comboSelectedMethod(int index){
    spinBoxBetta->setEnabled(false);
    switch(index){
    case METHOD_HYBRID_IN_OUT:
        spinBoxBetta->setEnabled(true);
        break;
    }
}

void MainWindow::slot_comboSelectedProbe(int index){
    labelPorbe4->setDisabled(false);
    spinBoxProbe4->setDisabled(false);

    switch(index){
    case PROBE_FUNCTION_CIRCLE:
        labelPorbe1->setText(" x0: ");
        labelPorbe2->setText(" y0: ");
        labelPorbe3->setText(" r: ");
        labelPorbe4->setText("");
        labelPorbe4->setDisabled(true);
        spinBoxProbe4->setDisabled(true);
        break;

    case PROBE_FUNCTION_SQUARE:
        labelPorbe1->setText(" x0: ");
        labelPorbe2->setText(" y0: ");
        labelPorbe3->setText(" h: ");
        labelPorbe4->setText(" w: ");
        break;

    }
    slot_spinBoxProbeChanged(0.0);
}

void MainWindow::on_actionErrorsDialog_triggered()
{
    dialogErrors->show();
}

void MainWindow::slot_spinBoxProbeChanged(double var){
    int minSize = measure.getSizeX();
    if(minSize > measure.getSizeY()) minSize = measure.getSizeY();

    double x0,y0,radius,h,w;

    x0 = spinBoxProbe1->value() * minSize;
    y0 = spinBoxProbe2->value() * minSize;
    h = spinBoxProbe3->value() * minSize;
    w = spinBoxProbe4->value() * minSize;
    radius = spinBoxProbe3->value() * minSize;

    plotOut->plot2D->clearItems();


    switch (comboSelectProbe->currentIndex()){
    case PROBE_FUNCTION_CIRCLE:
        QCPItemEllipse *ellipse;
        ellipse = new QCPItemEllipse(plotOut->plot2D);
        ellipse->setPen(QPen(QColor("green"),1,Qt::SolidLine,Qt::SquareCap,Qt::BevelJoin));

        ellipse->topLeft->setCoords(x0+radius,y0+radius);
        ellipse->bottomRight->setCoords(x0-radius,y0-radius);
        break;

    case PROBE_FUNCTION_SQUARE:
        QCPItemLine *line1 = new QCPItemLine(plotOut->plot2D);
        QCPItemLine *line2 = new QCPItemLine(plotOut->plot2D);
        QCPItemLine *line3 = new QCPItemLine(plotOut->plot2D);
        QCPItemLine *line4 = new QCPItemLine(plotOut->plot2D);
        line1->setPen(QPen(QColor("green"),1,Qt::SolidLine,Qt::SquareCap,Qt::BevelJoin));
        line2->setPen(QPen(QColor("green"),1,Qt::SolidLine,Qt::SquareCap,Qt::BevelJoin));
        line3->setPen(QPen(QColor("green"),1,Qt::SolidLine,Qt::SquareCap,Qt::BevelJoin));
        line4->setPen(QPen(QColor("green"),1,Qt::SolidLine,Qt::SquareCap,Qt::BevelJoin));

        line1->start->setCoords(x0-w,y0+h);
        line1->end->setCoords(x0+w,y0+h);
        line2->start->setCoords(x0-w,y0-h);
        line2->end->setCoords(x0+w,y0-h);
        line3->start->setCoords(x0+w,y0+h);
        line3->end->setCoords(x0+w,y0-h);
        line4->start->setCoords(x0-w,y0+h);
        line4->end->setCoords(x0-w,y0-h);
        break;
    }


    plotOut->plot2D->repaint();
    plotOut->plot2D->update();
    plotOut->plot2D->replot();
    return;
}

void MainWindow::preparePlotOut(){
    plotOut->plot2D->xAxis->setRange(0,measure.getSizeX());
    plotOut->plot2D->yAxis->setRange(0,measure.getSizeY());
    plotOut->plot2D->ColorMap->data()->clear();
    plotOut->plot2D->clearGraphs();
    plotOut->plot2D->replot();
    slot_comboSelectedProbe(0.0);
}

void MainWindow::on_actionOptionsDialog_triggered()
{
    dialogOptions->show();
}

void MainWindow::slot_recvOptionsData(s_options *sopt){
    *Options = *sopt;
    qDebug() << Options->save_directory;
}

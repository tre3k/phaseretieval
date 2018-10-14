#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QGridLayout *mainLayout = new QGridLayout();
    QWidget *mainWidget = new QWidget();

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

    tComplex2D measure;
    tFFT fft;
    fft.dfft2d(&InputImage,&measure);


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
}

void MainWindow::OpenImage(QString filename, tComplex2D *output){
    int sx,sy,tmp;

    double nx,ny;

    QImage *img = new QImage(filename);

    sx = img->width();
    sy = img->height();
    qDebug () << sx << " " << sy;

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

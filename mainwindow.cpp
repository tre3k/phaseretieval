#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QGridLayout *mainLayout = new QGridLayout();
    QWidget *mainWidget = new QWidget();


    tPlot2DCase *plot = new tPlot2DCase();
    plot->setMinimumSize(QSize(20,20));
    mainLayout->addWidget(plot);

    mainWidget->setLayout(mainLayout);
    this->setCentralWidget(mainWidget);

}

MainWindow::~MainWindow()
{
    delete ui;
}

#include "plotdialog.h"

PlotDialog::PlotDialog(QWidget *parent) : QDialog(parent){
    mainLayout = new QVBoxLayout();
    this->setLayout(mainLayout);

    menuBar = new QMenuBar;
    mainLayout->setMenuBar(menuBar);

    menuFile = new QMenu("&File");
    actionSaveImage = new QAction("&Save plot image");
    actionSaveData = new QAction("Save data");
    actionExit = new QAction("E&xit");
    menuFile->addAction(actionSaveImage);
    menuFile->addAction(actionSaveData);
    menuFile->addSeparator();
    menuFile->addAction(actionExit);

    menuBar->addMenu(menuFile);


    plot = new tPlot();
    mainLayout->addWidget(plot);
    this->setMinimumSize(300,200);

    connect(actionSaveData,SIGNAL(triggered()),
            this,SLOT(slot_actionSaveData()));
    connect(actionExit,SIGNAL(triggered()),
            this,SLOT(slot_actionExit()));

}

void PlotDialog::setAxiesLabel(QString LabelX,QString LabelY){
    plot->xAxis->setLabel(LabelX);
    plot->yAxis->setLabel(LabelY);
}

void PlotDialog::slot_SetAxiesLabel(QString LabelX,QString LabelY){
    setAxiesLabel(LabelX,LabelY);
}

void PlotDialog::addValue(double x,double y){
    plot->addNewValue(x,y,graph);
}

void PlotDialog::slot_addValue(double x,double y){
    addValue(x,y);
}

void PlotDialog::setGraph(int lgraph){
    graph = lgraph;
}

void PlotDialog::slot_actionSaveData(){
    QString filename = QFileDialog::getSaveFileName(NULL,"Save data","","*.txt");
    if(filename=="") return;

    QFile f(filename);
    QTextStream stream(&f);

    f.open(QIODevice::WriteOnly);

    for(int i=0;i<plot->graph(0)->dataCount();i++){
        stream << plot->graph(0)->dataMainKey(i) << "\t" << plot->graph(0)->dataMainValue(i) << "\n";
    }

    f.close();
    return;
}

void PlotDialog::slot_actionExit(){
    this->hide();
    return;
}

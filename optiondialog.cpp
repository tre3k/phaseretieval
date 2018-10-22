#include "optiondialog.h"

OptionDialog::OptionDialog(QWidget *parent) : QDialog (parent){
    sOptions = new s_options;

    QVBoxLayout *mainLayout = new QVBoxLayout();
    QFormLayout *layoutSavePath = new QFormLayout();

    textPathForSaveImg = new QLineEdit("./");
    buttonPathForSaveImg = new QPushButton("...");

    QHBoxLayout *layoutSaveLineAndButton = new QHBoxLayout();
    layoutSaveLineAndButton->addWidget(textPathForSaveImg);
    layoutSaveLineAndButton->addWidget(buttonPathForSaveImg);
    layoutSavePath->addRow("Path for save image: ",layoutSaveLineAndButton);


    buttonClose = new QPushButton("close");
    buttonOk = new QPushButton("ok");

    QHBoxLayout *layoutForButtonOkAndClose = new QHBoxLayout();
    layoutForButtonOkAndClose->addStretch(10);
    layoutForButtonOkAndClose->addWidget(buttonOk);
    layoutForButtonOkAndClose->addWidget(buttonClose);

    mainLayout->addLayout(layoutSavePath);
    mainLayout->addLayout(layoutForButtonOkAndClose);

    connect(buttonClose,SIGNAL(clicked()),
            this,SLOT(slot_pushCloseButton()));
    connect(buttonOk,SIGNAL(clicked()),
            this,SLOT(slot_pushOkButton()));
    connect(buttonPathForSaveImg,SIGNAL(clicked()),
            this,SLOT(slot_pushOpenButton()));

    this->setLayout(mainLayout);
}

void OptionDialog::slot_pushCloseButton(){
    this->hide();
    return;
}

void OptionDialog::slot_pushOkButton(){
    sOptions->save_directory = textPathForSaveImg->text();
    sOptions->image_format = IMAGE_FORMAT_JPG;
    sOptions->number_of_plots = 50;

    emit signal_sendOptionsData(sOptions);
    this->hide();
    return;
}

void OptionDialog::slot_pushOpenButton(){
    textPathForSaveImg->setText(QFileDialog::getExistingDirectory(0,"Open directory",textPathForSaveImg->text()));
    return;
}

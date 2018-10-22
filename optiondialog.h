#ifndef OPTIONDIALOG_H
#define OPTIONDIALOG_H

#include <QDialog>
#include <QWidget>
#include <QLayout>
#include <QFormLayout>
#include <QLineEdit>
#include <QButtonGroup>
#include <QPushButton>
#include <QLabel>
#include <QFileDialog>

enum{
    IMAGE_FORMAT_PNG,
    IMAGE_FORMAT_JPG,
    IMGGE_FORMAT_BMP
};

struct s_options{
    QString save_directory;
    int number_of_plots;
    int image_format;
};

class OptionDialog : public QDialog{
    Q_OBJECT
public:
    explicit OptionDialog(QWidget *parent = 0);
private:
    QLineEdit *textPathForSaveImg;
    QPushButton *buttonPathForSaveImg;

    QPushButton *buttonOk;
    QPushButton *buttonClose;

    s_options *sOptions;

private slots:
    void slot_pushCloseButton();
    void slot_pushOkButton();
    void slot_pushOpenButton();

signals:
    void signal_sendOptionsData(s_options *);

};

#endif

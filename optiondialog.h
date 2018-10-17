#ifndef OPTIONDIALOG_H
#define OPTIONDIALOG_H

#include <QDialog>
#include <QWidget>

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

};

#endif

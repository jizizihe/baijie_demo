#ifndef OPENFILE_H
#define OPENFILE_H

#include <QMainWindow>

namespace Ui {
class openfile;
}

class openfile : public QMainWindow
{
    Q_OBJECT

public:
    explicit openfile(QWidget *parent = 0);
    ~openfile();

private:
    Ui::openfile *ui;
};

#endif // OPENFILE_H

#ifndef TIMESET_H
#define TIMESET_H

#include <QMainWindow>

namespace Ui {
class timeset;
}

class timeset : public QMainWindow
{
    Q_OBJECT

public:
    explicit timeset(QWidget *parent = 0);
    ~timeset();

private:
    Ui::timeset *ui;
};

#endif // TIMESET_H

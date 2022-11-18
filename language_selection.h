#ifndef LANGUAGE_SELECTION_H
#define LANGUAGE_SELECTION_H

#include <QMainWindow>

namespace Ui {
class language_selection;
}

class language_selection : public QMainWindow
{
    Q_OBJECT

public:
    explicit language_selection(QWidget *parent = 0);
    ~language_selection();

private:
    Ui::language_selection *ui;
};

#endif // LANGUAGE_SELECTION_H

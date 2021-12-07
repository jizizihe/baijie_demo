#ifndef SIM_MODULE_H
#define SIM_MODULE_H

#include <QMainWindow>

namespace Ui {
class sim_module;
}

class sim_module : public QMainWindow
{
    Q_OBJECT

public:
    explicit sim_module(QWidget *parent = 0);
    ~sim_module();

private:
    Ui::sim_module *ui;
};

#endif // SIM_MODULE_H

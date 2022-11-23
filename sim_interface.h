#ifndef SIM_INTERFACE_H
#define SIM_INTERFACE_H

#include <QObject>
#include <QThread>
#include <QDebug>
#include <QProcess>
#include <QFile>
#include <QDir>
#include <QMovie>
#include <QInputDialog>
#include <QDialog>
#include <QMessageBox>
#include <QFileInfo>
#include <QHBoxLayout>
#include "gpio_interface.h"
#include "database.h"

enum EnumSimSignalType
{
    EnumSimDisconnectSignal = 1,
    EnumSimActivationSignal = 2,
    EnumSimDeleteSignal = 3,
    EnumSimConnectSignal = 4,
    EnumSimStatusSignal = 5,
    EnumSimModuleStatusSignal = 6,
};

class sim_interface: public QObject
{
    Q_OBJECT
public:
    explicit sim_interface(QObject *parent = nullptr);
    ~sim_interface();

    QString executeLinuxCmd(QString strCmd);
    QString simDisconnect();
    QString simConnect();
    QString getSimStatus();         // Connection network status
    QString getSimModuleStatus();   // Sim module status
    void setSimEnable(int flag);
};

#endif // SIM_INTERFACE_H

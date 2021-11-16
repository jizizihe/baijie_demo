#include "wifi_bt_interface.h"

wifi_bt_interface::wifi_bt_interface(QObject *parent) : QObject(parent)
{

}


QString wifi_bt_interface::executeLinuxCmd(QString strCmd)
{
    QProcess p;
    p.start("bash", QStringList() <<"-c" << strCmd);
    p.waitForFinished(-1);
    QString strResult = p.readAllStandardOutput();
    p.close();
    return strResult;
}

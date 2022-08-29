#ifndef DATABASE_H
#define DATABASE_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

class database
{
public:
    database();
    bool create_connection();
    bool create_table();

    bool insert_wifitable(QString name, QString passwd);
    bool insert_hotstop(QString name, QString passwd, QString wlan);
    bool insert_bluetooth(QString name,QString address);
    bool insert_ip_static(QString ipaddress);
    bool insert_gpio(QString gpio,QString state,QString value);

    bool select_table(QString name);
    QString select_by_name(QString tableName, QString name);
    bool update_table(QString tableName, QString name, QString passwd);
    bool delete_record_by_name(QString tableName, QString name);
    bool delete_table(QString tableName);

    QStringList hostpot_tabelshow();
    QStringList table_show(QString tablename);
    void table_debug(QString tablename);

};

#endif // DATABASE_H

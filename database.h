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

    bool insert_table1(QString table,QString unit1);
    bool insert_table2(QString table,QString unit1,QString unit2);
    bool insert_table3(QString table,QString unit1,QString unit2,QString unit3);

    bool select_table(QString name);
    QString select_by_name(QString tableName, QString name);
    bool update_wifitable(QString tableName, QString name, QString passwd);
    bool delete_record_by_name(QString tableName, QString name);
    bool delete_table(QString tableName);

    QStringList table_show(QString tablename);
    void table_debug(QString tablename);

};

#endif // DATABASE_H

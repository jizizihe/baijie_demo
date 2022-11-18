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
    bool createConnection();
    bool createTable();
    bool insertTableOne(QString table,QString unit1);
    bool insertTableTwo(QString table,QString unit1,QString unit2);
    bool insertTableThree(QString table,QString unit1,QString unit2,QString unit3);
    bool selectTable(QString name);
    bool updateWiFiTable(QString tableName, QString name, QString passwd);
    bool updateTableOne(QString tableName, QString name);
    bool deleteTableName(QString tableName, QString name);
    bool deleteTable(QString tableName);
    QStringList tableShow(QString tableName);
    QString selectTableName(QString tableName, QString name);
    void tableDebug(QString tableName);
};

#endif // DATABASE_H

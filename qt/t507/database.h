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
    bool insert_table(QString name, QString passwd);
    bool select_table(QString name);
    QString select_by_name(QString tableName, QString name);
    bool update_table(QString tableName, QString name, QString passwd);
    bool delete_record_by_name(QString tableName, QString name);
    bool delete_table(QString tableName);

};

#endif // DATABASE_H

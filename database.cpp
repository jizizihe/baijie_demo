#include "database.h"
#include <QFileInfo>
#include <QSqlRecord>

static int createFlag;

database::database()
{
    if(createFlag == 0)       //Create database only once
    {
        createConnection();
        createTable();
        createFlag++;
    }
}

bool database::createConnection()
{
    QSqlDatabase db;
    if(QSqlDatabase::contains("qt_sql_default_connection"))
        db = QSqlDatabase::database("qt_sql_default_connection");
    else
        db = QSqlDatabase::addDatabase("QSQLITE");

    db.setDatabaseName("/helperapp.db");
    if(!db.open())
    {
        qDebug() << "Error:database open failed!"<<db.lastError();
        return false;
    }
    else
    {
        return true;
    }
}

bool database::createTable()
{
    QSqlQuery query;
    QString createTableWifi = "create table wifiPasswd (name varchar(64) primary key,password varchar(32))";
    query.prepare(createTableWifi);
    if(query.exec())
    {
    }

    QString createTableHostpot = "create table hostpot (name varchar(64) primary key,password varchar(32),wlan varchar(64))";
    query.prepare(createTableHostpot);
    if(query.exec())
    {
    }

    QString createTableBluetooth = "create table bluetooth (name varchar(64) primary key,address varchar(32))";
    query.prepare(createTableBluetooth);
    if(query.exec())
    {
    }

    QString createTableIPStatic = "create table serial_port (name varchar(64) primary key)";
    query.prepare(createTableIPStatic);
    if(query.exec())
    {
    }

    QString createTableVoice = "create table voice (name varchar(64) primary key,filename varchar(64))";
    query.prepare(createTableVoice);
    if(query.exec())
    {
    }
    return true;
}

bool database::insertTableTwo(QString table, QString unit1, QString unit2)
{
    QSqlQuery query;
    QString insertSql = QString("insert into %1 values ('%2','%3')").arg(table).arg(unit1).arg(unit2);
    if(!query.exec(insertSql))
    {
        qDebug() << "Error: Failed to insert into table"<<query.lastError();
        return false;
    }
    else
    {
        return true;
    }
}

bool database::insertTableThree(QString table, QString unit1, QString unit2, QString unit3)
{
    QSqlQuery query;
    QString insertSql = QString("insert into %1 values ('%2','%3','%4')").arg(table).arg(unit1).arg(unit2).arg(unit3);
    if(!query.exec(insertSql))
    {
        qDebug() << "Error: Failed to insert into table"<<query.lastError();
        return false;
    }
    else
    {
        return true;
    }
}

bool database::insertTableOne(QString table,QString unit1)
{
    QSqlQuery query;
    QString insertSql = QString("insert into %1 values ('%2')").arg(table).arg(unit1);
    if(!query.exec(insertSql))
    {
        qDebug() << "Error: Failed to insert into table"<< query.lastError();
        return false;
    }
    else
    {
        return true;
    }
}

bool database::selectTable(QString name)
{
    QSqlQuery query;
    QString selectSql = QString("select * from wifiPasswd where name = '%1'").arg(name);
    if(!query.exec(selectSql))
    {
        qDebug() << "Error: Failed select from table."<<query.lastError();
        return false;
    }
    else
    {
        qDebug() << "select from table succeed!";
        while(query.next())
        {
            //qDebug() << "--LINE--: " << __LINE__<< query.value(0).toString()<<query.value(1).toString();
        }
        return true;
    }
}

QString database::selectTableName(QString tableName, QString name)
{
    QSqlQuery query;
    QString selectTableName = QString("select * from %1 where name='%2';").arg(tableName).arg(name);
    if(!query.exec(selectTableName))
    {
        qDebug() << "Error: Failed select from table by name."<<query.lastError();
        return "false";
    }
    else
    {
        if(query.first())
        {
            return query.value(1).toString();
        }
    }
    return "false";
}

bool database::updateWiFiTable(QString tableName, QString name, QString passwd)
{
    QSqlQuery query;
    QString updateSql = QString("update %1 set password='%2' where name='%3';").arg(tableName).arg(passwd).arg(name);
    if(!query.exec(updateSql))
    {
        qDebug() << "Error: Failed update table by name."<<query.lastError();
        return false;
    }
    else
    {
        return true;
    }
    return false;
}

bool database::deleteTableName(QString tableName, QString name)
{
    QSqlQuery query;
    QString deleteTableName = QString("delete from %1 where name='%2';").arg(tableName).arg(name);
    if(!query.exec(deleteTableName))
    {
        qDebug() << "Error: Failed delete record by name."<<query.lastError();
        return false;
    }
    else
    {
        return true;
    }
    return false;
}

bool database::deleteTable(QString tableName)
{
    QSqlQuery query;
    QString deleteTable = QString("drop table %1;").arg(tableName);
    if(!query.exec(deleteTable))
    {
        qDebug() << "Error: Failed delete record by name."<<query.lastError();
        return false;
    }
    else
    {
        return true;
    }
    return false;
}

QStringList database::tableShow(QString tableName)      //Return database table data to QStringList
{
    QSqlQuery query;
    QString strr =QString("select * from %1").arg(tableName);
    query.exec(strr);
    QStringList list;
    QSqlRecord rec = query.record();
    QString str;
    while (query.next())
    {
        for(int index = 0;index < rec.count();index++)
        {
            str = str + query.value(index).toString() + ",";
        }
        list = str.split(",");
        list.removeAll("");
    }
    return list;
}

void database::tableDebug(QString tableName)      //qDebug the database table data
{
    QSqlQuery query;
    QString str = QString("select * from %1").arg(tableName);
    query.exec(str);
    QSqlRecord rec = query.record();
    while(query.next())
    {
        for(int index = 0; index < rec.count(); index++)
        {
            qDebug() << query.value(index);
        }
    }
}

#include "database.h"
#include <QFileInfo>
#include <QSqlRecord>
static int create_falg;

database::database()
{
    if(create_falg == 0)
    {
        create_connection();
        create_table();
        create_falg++;
    }
}

bool database::create_connection()
{
    QSqlDatabase db;
    if(QSqlDatabase::contains("qt_sql_default_connection"))
      db = QSqlDatabase::database("qt_sql_default_connection");
    else
      db = QSqlDatabase::addDatabase("QSQLITE");

    db.setDatabaseName("/helperapp.db"); //如果本目录下没有该文件,则会在本目录下生成,否则连接该文件

    if(!db.open())
    {
        qDebug() << "Error:database open failed!"<<db.lastError();
        return false;
    }
    else
    {
       // qDebug() << "open succeed!";
        return true;
    }
}

bool database::create_table()
{
    delete_table("serial_port");
    QSqlQuery query;
    QString create_table_wifi = "create table wifiPasswd (name varchar(64) primary key,password varchar(32))";
    query.prepare(create_table_wifi);
    if(query.exec())
    {
        //qDebug() << "create table_wifi succeed!";
    }

    QString create_table_hostpot = "create table hostpot (name varchar(64) primary key,password varchar(32),wlan varchar(64))";
    query.prepare(create_table_hostpot);
    if(query.exec())
    {
        //qDebug() << "create table_hostpot succeed!";
    }

    QString create_table_bluetooth = "create table bluetooth (name varchar(64) primary key,address varchar(32))";
    query.prepare(create_table_bluetooth);
    if(query.exec())
    {
        //qDebug() << "create table_bluetooth succeed!";
    }

    QString create_table_ip_static = "create table serial_port (name varchar(64) primary key)";
    query.prepare(create_table_ip_static);
    if(query.exec())
    {
      //  qDebug() << "create table_ipstatic succeed!";
    }

    QString create_table_voice = "create table voice (name varchar(64) primary key,filename varchar(64))";
    query.prepare(create_table_voice);
    if(query.exec())
    {
      //  qDebug() << "create table_voice succeed1!";
    }
    return true;
}

bool database::insert_table2(QString table, QString unit1, QString unit2)
{
    QSqlQuery query;

    QString insert_sql = QString("insert into %1 values ('%2','%3')").arg(table).arg(unit1).arg(unit2);
   // qDebug()<< "insert_sql:" << insert_sql;
    if(!query.exec(insert_sql))
    {
        qDebug() << "Error: Failed to insert into table"<<query.lastError();
        return false;
    }
    else
    {
       // qDebug() << "insert into table succed!";
        return true;
    }
}

bool database::insert_table3(QString table, QString unit1, QString unit2, QString unit3)
{
    QSqlQuery query;
    QString insert_sql = QString("insert into %1 values ('%2','%3','%4')").arg(table).arg(unit1).arg(unit2).arg(unit3);
   // qDebug()<< "insert_sql:" << insert_sql;
    if(!query.exec(insert_sql))
    {
        qDebug() << "Error: Failed to insert into table"<<query.lastError();
        return false;
    }
    else
    {
   //     qDebug() << "insert into table succed!";
        return true;
    }
}

bool database::insert_table1(QString table,QString unit1)
{
    QSqlQuery query;
    QString insert_sql = QString("insert into %1 values ('%2')").arg(table).arg(unit1);
   // qDebug() << "insert_sql" << insert_sql;
    if(!query.exec(insert_sql))
    {
        qDebug() << "Error: Failed to insert into table"<< query.lastError();
        return false;
    }
    else
    {
       // qDebug() << "insert into table succed!";
        return true;
    }
}

bool database::select_table(QString name)
{
    QSqlQuery query;

    //qDebug() <<"--LINE--: " << __LINE__<< name;
    QString select_sql = QString("select * from wifiPasswd where name = '%1'").arg(name);
    if(!query.exec(select_sql))
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

QString database::select_by_name(QString tableName, QString name)
{
    QSqlQuery query;
    QString select_by_name_sql = QString("select * from %1 where name='%2';").arg(tableName).arg(name);
    //qDebug() << "--LINE--: " << __LINE__<< "select_by_name_sql="<<select_by_name_sql;
    if(!query.exec(select_by_name_sql))
    {
        qDebug() << "Error: Failed select from table by name."<<query.lastError();
        return "false";
    }
    else
    {
        if(query.first())
        {
          //  qDebug() << "select from table succeed!";
          //  qDebug() << "--LINE--: " << __LINE__<< query.value(0).toString()<<query.value(1).toString();
            return query.value(1).toString();
        }
    }
    return "false";
}

bool database::update_wifitable(QString tableName, QString name, QString passwd)
{
    QSqlQuery query;
    QString update_sql = QString("update %1 set password='%2' where name='%3';").arg(tableName).arg(passwd).arg(name);
  //  qDebug() << update_sql;
    if(!query.exec(update_sql))
    {
        qDebug() << "Error: Failed update table by name."<<query.lastError();
        return false;
    }
    else
    {
      //  qDebug() << "update table succeed!";
        return true;
    }
    return false;
}

bool database::delete_record_by_name(QString tableName, QString name)
{
    QSqlQuery query;
    QString delete_record_sql = QString("delete from %1 where name='%2';").arg(tableName).arg(name);
    if(!query.exec(delete_record_sql))
    {
        qDebug() << "Error: Failed delete record by name."<<query.lastError();
        return false;
    }
    else
    {
        //qDebug() << "delete record succeed!";
        return true;
    }
    return false;
}

bool database::delete_table(QString tableName)
{
    QSqlQuery query;
    QString delete_table_sql = QString("drop table %1;").arg(tableName);
    if(!query.exec(delete_table_sql))
    {
    //    qDebug() << "Error: Failed delete record by name."<<query.lastError();
        return false;
    }
    else
    {
       // qDebug() << "delete record succeed!";
        return true;
    }
    return false;
}

QStringList database::table_show(QString tablename)
{
    QSqlQuery query;
    QString strr =QString("select * from %1").arg(tablename);
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

void database::table_debug(QString tablename)
{
    QSqlQuery query;
    QString str = QString("select * from %1").arg(tablename);
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

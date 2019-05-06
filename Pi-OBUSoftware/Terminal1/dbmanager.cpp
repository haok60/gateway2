#include "dbmanager.h"
DbManager::DbManager()
{

    m_db = QSqlDatabase::addDatabase("QSQLITE");
       m_db.setDatabaseName("/home/nghiemdinhhao/gateway/Pi-OBUSoftware/build/WSAN/data.db");

       if (!m_db.open())
       {
          qDebug() << "Error: connection with database fail";
       }
       else
       {
          qDebug() << "Database: connection ok";
       }
}
bool DbManager::insertdata(QString date, int node, int value)
{
    bool success = false;
    QSqlQuery query;
    query.prepare("INSERT INTO data(Time,Node,Value) VALUES (:date,:node,:value)");
    query.bindValue(":date", date);
    query.bindValue(":node",node);
    query.bindValue(":value",value);
    if(query.exec())
       {
           success = true;
       }
    return success;

}
bool DbManager::insertrecentdata(QString date, int node, int value)
{
    bool success = false;
    QSqlQuery query;
    query.prepare("INSERT INTO recentData(Time,Node,Value) VALUES (:date,:node,:value)");
    query.bindValue(":date", date);
    query.bindValue(":node",node);
    query.bindValue(":value",value);
    if(query.exec())
       {
           success = true;
       }
    return success;
}
QStringList DbManager::getdata(int node)
{
    QStringList list;
    QSqlQuery query;
    QString queryString = "SELECT Time,Value FROM data WHERE Node = "+QString::number(node);
    qDebug()<<queryString;
    query.exec(queryString);
    while (query.next()) {
        QSqlRecord record = query.record();
        QString time = record.value(0).toString();
        list.append(time);
        QString Value = QString::number(record.value(1).toDouble());
        list.append(Value);
        qDebug() << "Time : "<<record.value(1).toDouble();
    }
    return list;
}
QStringList DbManager::getrecentdata(int node)
{
    QStringList list;
    QSqlQuery query;
    QString queryString = "SELECT Time,Value FROM data WHERE Node =" +QString::number(node);
    qDebug()<<queryString;
    query.exec(queryString);
    while (query.next()) {
        QSqlRecord record = query.record();
        QString time = record.value(0).toString();
        list.append(time);
        QString Value = QString::number(record.value(1).toDouble());
        list.append(Value);
        qDebug() << "Time : "<<record.value(1).toDouble();
    }
    return list;
}
bool DbManager::deletedata(int node)
{
    bool success = false;
    QSqlQuery query;
    query.prepare("DELETE FROM data WHERE Node = :node");
    query.bindValue(":node",node);
    query.exec();
    if(query.exec())
       {
           success = true;
       }
    return success;
}
bool DbManager::deleterecentdata(int node)
{
    bool success = false;
    QSqlQuery query;
    query.prepare("DELETE FROM data WHERE Node = :node");
    query.bindValue(":node",node);
    query.exec();
    if(query.exec())
       {
           success = true;
       }
    return success;
}

#ifndef DBMANAGER_H
#define DBMANAGER_H
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QtSql/QSqlField>
#include <QtSql/QSqlRecord>
#include <QtSql/QSqlDriver>
#include <QStringList>
#include <QDebug>
class DbManager
{
public:
    DbManager();
    QStringList getdata(int);
    QStringList getrecentdata(int);
    bool insertdata(QString, int, int);
    bool insertrecentdata(QString, int, int);
    bool deletedata(int);
    bool deleterecentdata(int);


private:
    QSqlDatabase m_db;
};

#endif // DBMANAGER_H

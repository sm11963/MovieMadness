#include "connection.h"

Connection::Connection(QObject *parent) :
    QObject(parent)
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("192.168.0.10");
    db.setDatabaseName("Movies");
    db.setUserName("MovieMonitor");
    db.setPassword("movRus8734");
    dbOpen = db.open();
    if (dbOpen)
        QSqlQuery query("CREATE TABLE IF NOT EXISTS movies "
                              "(id int NOT NULL AUTO_INCREMENT, "
                              "name varchar(255) NOT NULL, "
                              "filename varchar(255) NOT NULL, "
                              "imbd varchar(255), "
                              "PRIMARY KEY (id))");
}


void Connection::addMovie(QString name, QString filename)
{
    if(filename.isEmpty() || !dbOpen)
        return;

    QSqlQuery query;
    query.prepare("INSERT INTO movies (name, filename) "
                  "VALUES (:name, :filename)");
    query.bindValue(":name", name);
    query.bindValue(":filename", filename);
    query.exec();
}

bool Connection::inDatabase(QString filename)
{
    if(filename.isEmpty() || !dbOpen)
        return false;

    QSqlQuery query;
    query.prepare("SELECT COUNT(filename) FROM movies WHERE filename=:filename");
    query.bindValue(":filename", filename);
    query.exec();
    query.next();
    return query.value(0).toInt() > 0;
}

bool Connection::isApproved(QString filename)
{
    if(!dbOpen)
        return false;

    QSqlQuery query;
    query.prepare("SELECT imbd FROM movies WHERE filename=:filename");
    query.bindValue(":filename",filename);
    query.exec();
    if(query.next())
        return !query.value(0).isNull();
    else
        return false;
}

void Connection::removeRow(QString filename)
{
    if(filename.isEmpty() || !dbOpen)
        return;

    QSqlQuery query;
    query.prepare("DELETE FROM movies WHERE filename=:filename");
    query.bindValue(":filename", filename);
    query.exec();
}

void Connection::updateRow(QString oldfile, QString newfile, QString newname)
{
    if(!dbOpen)
        return;

    QSqlQuery query;
    query.prepare("UPDATE movies SET filename=:newname "
                  "WHERE filename=:oldname");
    query.bindValue(":oldname", oldfile);
    query.bindValue(":newname", newfile);
    query.exec();
    qDebug() << query.lastError();

    if(!isApproved(newfile))
    {
        query.prepare("UPDATE movies SET name=:name "
                      "WHERE filename=:filename");
        query.bindValue(":name", newname);
        query.bindValue(":filename", newfile);
        query.exec();
    }
}

QStringList Connection::getFilenames()
{
    QStringList resultList;
    if(!dbOpen)
        return resultList;

    QSqlQuery query("SELECT filename FROM movies");
    while(query.next())
    {
        resultList.append(query.value(0).toString());
    }
    return resultList;
}

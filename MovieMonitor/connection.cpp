#include "connection.h"

Connection::Connection(QObject *parent) :
    QObject(parent)
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("192.168.0.10");
    db.setDatabaseName("Movies");
    db.setUserName("MovieMonitor");
    db.setPassword("movRus8734");
    db.setConnectOptions("MYSQL_OPT_RECONNECT=1;");
    QLOG_INFO() << "Attempting to connect to database...";
    if (db.open())
    {
        QLOG_INFO() << "Connection successful";
        QSqlQuery query("CREATE TABLE IF NOT EXISTS movies "
                              "(id int NOT NULL AUTO_INCREMENT, "
                              "name varchar(255) NOT NULL, "
                              "filename varchar(255) NOT NULL, "
                              "imbd varchar(255), "
                              "PRIMARY KEY (id))");
    }
    else
    {
        QLOG_FATAL() << "Connection to database was unsuccessful";
        throw new std::exception;
    }
}


void Connection::addMovie(QString name, QString filename)
{
    if(filename.isEmpty() || !QSqlDatabase::database().isValid())
    {
        QLOG_WARN() << "Database is not available...";
        return;
    }

    QSqlQuery query;
    query.prepare("INSERT INTO movies (name, filename) "
                  "VALUES (:name, :filename)");
    query.bindValue(":name", name);
    query.bindValue(":filename", filename);
    query.exec();

    if(query.lastError().type() != QSqlError::NoError)
        QLOG_ERROR() << "SQL ERROR: " + query.lastError().text();
}

bool Connection::inDatabase(QString filename)
{
    if(filename.isEmpty() || !QSqlDatabase::database().isValid())
    {
        QLOG_WARN() << "Database is not available...";
        return false;
    }

    QSqlQuery query;
    query.prepare("SELECT COUNT(filename) FROM movies WHERE filename=:filename");
    query.bindValue(":filename", filename);
    query.exec();
    query.next();
    if(query.lastError().type() != QSqlError::NoError)
        QLOG_ERROR() << "SQL ERROR: " + query.lastError().text();

    return query.value(0).toInt() > 0;
}

bool Connection::isApproved(QString filename)
{
    if(!QSqlDatabase::database().isValid())
    {
        QLOG_WARN() << "Database is not available...";
        return false;
    }

    QSqlQuery query;
    query.prepare("SELECT imbd FROM movies WHERE filename=:filename");
    query.bindValue(":filename",filename);
    query.exec();
    if(query.lastError().type() != QSqlError::NoError)
        QLOG_ERROR() << "SQL ERROR: " + query.lastError().text();

    if(query.next())
        return !query.value(0).isNull();
    else
        return false;
}

void Connection::removeRow(QString filename)
{
    if(filename.isEmpty() || !QSqlDatabase::database().isValid())
    {
        QLOG_WARN() << "Database is not available...";
        return;
    }

    QSqlQuery query;
    query.prepare("DELETE FROM movies WHERE filename=:filename");
    query.bindValue(":filename", filename);
    query.exec();
    if(query.lastError().type() != QSqlError::NoError)
        QLOG_ERROR() << "SQL ERROR: " + query.lastError().text();
}

void Connection::updateRow(QString oldfile, QString newfile, QString newname)
{
    if(!QSqlDatabase::database().isValid())
    {
        QLOG_WARN() << "Database is not available...";
        return;
    }

    QSqlQuery query;
    query.prepare("UPDATE movies SET filename=:newname "
                  "WHERE filename=:oldname");
    query.bindValue(":oldname", oldfile);
    query.bindValue(":newname", newfile);
    query.exec();
    if(query.lastError().type() != QSqlError::NoError)
        QLOG_ERROR() << "SQL ERROR: " + query.lastError().text();

    if(!isApproved(newfile))
    {
        query.prepare("UPDATE movies SET name=:name "
                      "WHERE filename=:filename");
        query.bindValue(":name", newname);
        query.bindValue(":filename", newfile);
        query.exec();
        if(query.lastError().type() != QSqlError::NoError)
            QLOG_ERROR() << "SQL ERROR: " + query.lastError().text();
    }
}

QStringList Connection::getFilenames()
{
    QStringList resultList;
    if(!QSqlDatabase::database().isValid())
    {
        QLOG_WARN() << "Database is not available...";
        return resultList;
    }

    QSqlQuery query("SELECT filename FROM movies");
    if(query.lastError().type() != QSqlError::NoError)
        QLOG_ERROR() << "SQL ERROR: " + query.lastError().text();

    while(query.next())
    {
        resultList.append(query.value(0).toString());
    }
    return resultList;
}

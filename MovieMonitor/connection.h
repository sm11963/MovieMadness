#ifndef CONNECTION_H
#define CONNECTION_H

#include <QObject>
#include <QSql>
#include <QSqlDatabase>
#include <QStringList>
#include <QString>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QVariant>
#include <QsLog/QsLog.h>

class Connection : public QObject
{
    Q_OBJECT
public:
    explicit Connection(QObject *parent = 0);    
    void addMovie(QString name, QString filename);
    bool inDatabase(QString filename);
    bool isApproved(QString filename);
    void removeRow(QString filename);
    QStringList getFilenames();

    void updateRow(QString oldfile, QString newfile, QString newname);
signals:

public slots:
    
private:
    bool dbOpen;

};

#endif // CONNECTION_H

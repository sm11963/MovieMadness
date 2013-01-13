#ifndef MONITOR_H
#define MONITOR_H

#include <QObject>
#include <QDir>
#include <QFileInfoList>
#include "connection.h"

#define DIR_PATH "/home/smiller/Dropbox/Drop Docs/2012-2013/RCPRS"

class Monitor : public QObject
{
    Q_OBJECT
public:
    explicit Monitor(QObject *parent = 0);
    void indexDir(QString path);
    void purgeMissing(QString path);
    
signals:
    
public slots:

private:
    Connection *dConnect;
    
};

#endif // MONITOR_H

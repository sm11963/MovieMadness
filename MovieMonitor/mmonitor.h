#ifndef MMONITOR_H
#define MMONITOR_H

#include <QThread>
#include "notifyworker.h"
#include "connection.h"
#include <QDir>
#include <QFileInfoList>
#include <QFileInfo>
#include <QQueue>
#include <QTimer>

class mmonitor : public QObject
{
    Q_OBJECT
public:
    explicit mmonitor(QString dir, QObject *parent = 0);
    void indexDir(QString path);
    void purgeMissing(QString path);

public slots:
    void processEvent(FileEvent event);
    void checkQueue();
    void start();

protected:

private:
    bool isMovieFile(QString filename);
    Connection *dConnect;
    NotifyWorker *watcher;
    QThread      *watcherThread;
    QString watch_dir;
    QQueue<FileEvent> *wQueue;
    static QStringList extensions;
    
};

#endif // MMONITOR_H

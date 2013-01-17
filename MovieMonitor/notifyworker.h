#ifndef NOTIFYWORKER_H
#define NOTIFYWORKER_H

#include <QObject>
#include "inotify-cxx/inotify-cxx.h"
#include "QsLog/QsLog.h"

typedef struct {
    QString name;
    QString type;
    int cookie;
} FileEvent;

class NotifyWorker : public QObject
{
    Q_OBJECT

public:
    explicit NotifyWorker(QString dir, QObject *parent = 0);
    
signals:
    void fEvent(FileEvent event);

public slots:
    void startWatching();

private:
    QString watch_dir;
};

#endif // NOTIFYWORKER_H

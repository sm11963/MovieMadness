#ifndef QTINOTIFY_H
#define QTINOTIFY_H

#include <QObject>
#include <sys/inotify.h>
#include <QQueue>
#include <QDebug>

class QtInotify : public QObject
{
    Q_OBJECT
public:
    explicit QtInotify(QObject *parent = 0);
    
signals:
    
public slots:
    
private:
    void process_inotify_events(int fd);
    int event_check (int fd);
    int read_events(int fd);

    QQueue<struct inotify_event> event_queue;
};

#endif // QTINOTIFY_H

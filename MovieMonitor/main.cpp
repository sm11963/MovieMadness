#include <QCoreApplication>
#include "connection.h"
#include "monitor.h"
#include "inotify-cxx/inotify-cxx.h"
#include "mmonitor.h"
#include <QDebug>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QStringList arguements = a.arguments();
    QString watch_dir;

    if(arguements.length() > 1)
        watch_dir = arguements.at(1);
    else
        watch_dir = "/storage/media/Movies";

    mmonitor monitor(watch_dir);
    monitor.start();

    return a.exec();
}

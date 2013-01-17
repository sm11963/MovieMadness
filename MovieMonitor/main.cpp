#include <QCoreApplication>
#include "connection.h"
#include "monitor.h"
#include "inotify-cxx/inotify-cxx.h"
#include "mmonitor.h"
#include "QsLog/QsLog.h"
#include "QsLog/QsLogDest.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QStringList arguements = a.arguments();
    QString watch_dir;

    if(arguements.length() > 1)
        watch_dir = arguements.at(1);
    else
        watch_dir = "/storage/media/Movies";

    QsLogging::Logger& logger = QsLogging::Logger::instance();
    logger.setLoggingLevel(QsLogging::TraceLevel);
    const QString sLogPath(QDir(a.applicationDirPath()).filePath("MovieMonitor.log"));
    QsLogging::DestinationPtr fileDestination(QsLogging::DestinationFactory::MakeFileDestination(sLogPath));
    logger.addDestination(fileDestination.get());

    QLOG_INFO() << "Program Started: Watching directory: " + watch_dir;

    mmonitor monitor(watch_dir);
    monitor.start();

    return a.exec();
}

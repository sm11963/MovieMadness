#include "mmonitor.h"

QStringList mmonitor::extensions = QStringList() << "mkv" << "avi" << "mp4"
                                          << "iso" << "thd" << "wmv" << "m4v";

mmonitor::mmonitor(QString dir, QObject *parent) :
    QObject(parent), watch_dir(dir)
{
    dConnect = new Connection();
    QDir d(watch_dir);
    if(!d.exists() || !d.isReadable())
    {
        QLOG_FATAL() << "Directory '" + dir + "' is not readable.";
        throw new std::exception;
    }

    watcher  = new NotifyWorker(watch_dir);
    watcherThread = new QThread();
    wQueue = new QQueue<FileEvent>();
}

void mmonitor::indexDir(QString path)
{
    QDir movieDir(path);
    QFileInfoList files = movieDir.entryInfoList(QDir::Files);

    foreach(QFileInfo file, files)
    {
        if(!dConnect->inDatabase(file.fileName()))
            dConnect->addMovie(file.completeBaseName(), file.fileName());
    }
}

void mmonitor::purgeMissing(QString path)
{
    QDir movieDir(path);
    QStringList files = movieDir.entryList(QDir::Files);
    QStringList dFilenames = dConnect->getFilenames();
    foreach(QString filename, dFilenames)
    {
        if(!files.contains(filename))
            dConnect->removeRow(filename);
    }
}

void mmonitor::processEvent(FileEvent event)
{
    QLOG_TRACE() << "Processing Event; Type: " + event.type + "; Name: " + event.name;
    if(isMovieFile(event.name))
    {
        if(event.type.contains("IN_DELETE"))
        {
            dConnect->removeRow(event.name);
        }
        else if(event.type.contains("IN_CREATE"))
        {
            QFileInfo file = QFileInfo(watch_dir+"/"+event.name);
            if (file.isFile())
            {
                dConnect->addMovie(file.completeBaseName(), file.fileName());
            }
        }
        else if(event.type.contains("IN_MOVED_FROM") ||
                event.type.contains("IN_MOVED_TO"))
        {
            if(wQueue->isEmpty())
                QTimer::singleShot(750, this, SLOT(checkQueue()));
            wQueue->enqueue(event);
        }
    }
}

void mmonitor::checkQueue()
{
    while(!wQueue->isEmpty())
    {
        FileEvent e = wQueue->dequeue();

        int i = 0;
        bool isMove = false;
        FileEvent match;
        QString new_name, old_name;

        while(!isMove && i < wQueue->length())
        {
            if(wQueue->at(i).cookie == e.cookie)
            {
                isMove = true;
                if(e.type.contains("IN_MOVED_TO"))
                {
                    new_name = e.name;
                    old_name = wQueue->at(i).name;
                }
                else
                {
                    new_name = wQueue->at(i).name;
                    old_name = e.name;
                }
                wQueue->removeAt(i);
            }
            i++;
        }

        if(isMove)
        {
            QFileInfo file = QFileInfo(watch_dir+"/"+new_name);
            if (file.isFile())
            {
                dConnect->updateRow(old_name, new_name, file.completeBaseName());
            }
        }
        else
        {
            if (e.type.contains("IN_MOVED_TO"))
            {
                QFileInfo file = QFileInfo(watch_dir+"/"+e.name);
                if (file.exists())
                {
                   dConnect->addMovie(file.completeBaseName(), file.fileName());
                }
            }
            else
            {
                dConnect->removeRow(e.name);
            }
        }
    }
}

void mmonitor::start()
{
    indexDir(watch_dir);
    purgeMissing(watch_dir);

    qRegisterMetaType<FileEvent>("FileEvent");
    connect(watcher, SIGNAL(fEvent(FileEvent)),
            this, SLOT(processEvent(FileEvent)));

    watcher->moveToThread(watcherThread);

    watcher->connect(watcherThread, SIGNAL(started()), watcher, SLOT(startWatching()));
    watcherThread->start();
}

bool mmonitor::isMovieFile(QString filename)
{
     QFileInfo file(watch_dir+"/"+filename);
     if(!file.exists())
         return false;

     QString ext = file.suffix();
     return extensions.contains(ext, Qt::CaseInsensitive);
}

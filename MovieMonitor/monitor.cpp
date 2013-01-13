#include "monitor.h"

Monitor::Monitor(QObject *parent) :
    QObject(parent)
{
    //dConnect = new Connection();
    //indexDir(DIR_PATH);
    //purgeMissing(DIR_PATH);
}

void Monitor::indexDir(QString path)
{
    QDir movieDir(path);
    QFileInfoList files = movieDir.entryInfoList(QDir::Files);

    foreach(QFileInfo file, files)
    {
        qDebug() << file.fileName();
        if(!dConnect->inDatabase(file.fileName()))
            dConnect->addMovie(file.baseName(), file.fileName());
    }
}

void Monitor::purgeMissing(QString path)
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

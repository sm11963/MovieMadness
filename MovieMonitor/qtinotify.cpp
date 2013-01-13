#include "qtinotify.h"

QtInotify::QtInotify(QObject *parent) :
    QObject(parent)
{
    event_queue = QQueue<struct inotify_event>();
    QString dirname = "/User/sm11963";
    QByteArray ba = dirname.toLocal8Bit();
    const char* dirname_char = ba.data();
    const long watch_mask = (IN_CREATE | IN_DELETE | IN_MOVED_FROM | IN_MOVED_TO);

    int inotify_fd;

    keep_running = 1;

    inotify_fd = inotify_init();

    if (inotify_fd < 0)
        qDebug() << "Error at inotify_init()";

    if(inotify_fd > 0)
    {
        int wd;
        wd = inotify_add_watch(fd, dirname_char, watch_mask);

    }
}

void QtInotify::process_inotify_events(int fd)
{
    while (keep_running)
    {
        if (event_check(fd) > 0)
        {
            int r = read_events(fd);
        }
    }
}

int QtInotify::event_check(int fd)
{
    fd_set rfds;
    FD_ZERO (&rfds);
    FD_SET (fd, &rfds);
    return select (FD_SETSIZE, &rfds, NULL, NULL, NULL);
}

int QtInotify::read_events(int fd)
{
    ssize_t r;

    r = read()
}



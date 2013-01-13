#include "notifyworker.h"

NotifyWorker::NotifyWorker(QString dir, QObject *parent) :
    QObject(parent), watch_dir(dir)
{
}

void NotifyWorker::startWatching()
{
    Inotify notify;

    int32_t watch_mask = (IN_CREATE | IN_DELETE | IN_MOVED_FROM | IN_MOVED_TO);
    QByteArray dir_str = watch_dir.toLocal8Bit();
    InotifyWatch iw(dir_str.data(), watch_mask);

    notify.Add(iw);

    for(;;)
    {
        notify.WaitForEvents();

        size_t count = notify.GetEventCount();

        while(count > 0)
        {
            InotifyEvent event;
            bool got_event = notify.GetEvent(&event);

            if(got_event)
            {
                FileEvent e;
                std::string s;
                event.DumpTypes(s);

                e.type = QString::fromUtf8(s.c_str());
                e.cookie = event.GetCookie();
                e.name = QString(event.GetName().c_str());

                if(!e.type.contains("IN_ISDIR"))
                {
                    emit fEvent(e);
                }
            }

            count--;
        }
    }
}

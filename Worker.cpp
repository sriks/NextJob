/*!
  \class Worker
  \brief Worker thread to do initail job data loading. This includes parsing data and populating data structures.
         This is primarily used for initial loading purpose only.
  **/

#include <QDebug>
#include <QVariantMap>
#include <QFile>
#include <QStringListIterator>
#include "Worker.h"
#include "rssmanager.h"
#include "rssparser.h"
#include "JobManager.h"
#include "Constants.h"
#include "JobManagerPrivate.h"

//#define SIMULATE_LONG_RUNNING_TASK

struct WorkerPrivate {
    JobManagerPrivate* master;
    Worker::TaskType task;
    QList< QVariantMap > favs;
    QList< QVariantMap > alertKeys;
    QStringList jobInfoKeys; // keys to load

    WorkerPrivate() {
        jobInfoKeys << NJ_PROP_KEY_TITLE << NJ_PROP_KEY_EMPNAME << NJ_PROP_KEY_LOCATION << NJ_PROP_KEY_DESCRIPTION
                    << NJ_PROP_KEY_URL << NJ_PROP_KEY_DATE << NJ_PROP_KEY_SOURCE;
    }

    void doTask() {
        if(Worker::RestoreState == task) {
            loadFavorites();
            loadAlerts();
#ifdef SIMULATE_LONG_RUNNING_TASK
            qDebug()<<Q_FUNC_INFO<<"SIMULATING  LONG RUNNING TASK";
            for(int i=0;i<10;++i) {
                for(int j=0;j<1000;++j) {
                    qDebug()<<Q_FUNC_INFO<<j;
                }
            }
#endif
       } else {
            qWarning()<<Q_FUNC_INFO<<"Invalid task "<<task;
        }
    }

    QString favFilePath() const {
        static QString path;
        if(path.isEmpty())
            path = JobManager::instance()->feedManager()->storagePath() + "/" + FAV_FILENAME;
        return path;
    }

    void loadFavorites() {
        static bool restored = false;
        if(restored)
            return;
        QFile f(favFilePath());
        if(f.open(QIODevice::ReadOnly)) {
            RSSParser* p = new RSSParser;
            p->setSource(&f);
            int c = p->count();
            qDebug()<<Q_FUNC_INFO<<c;
            for(int i=0;i<c;++i) {
                QVariantMap key;
                QStringListIterator iter(jobInfoKeys);
                while(iter.hasNext()) {
                    QString k = iter.next();
                    key.insert(k,p->itemElement(i,k));
                }
                favs.append(key);
            }
            delete p;
            restored = true;
            f.close();
        } else {
            qWarning()<<Q_FUNC_INFO<<"Unable to open "<<favFilePath();
        }

    }

    QVariantMap convertToVariantMap(FeedUserData userdata) {
        QVariantMap map;
        QMapIterator<QString,QString> iter(userdata);
        while(iter.hasNext()) {
            iter.next();
            map.insert(iter.key(),QVariant(iter.value()));
        }
        return map;
    }

    void loadAlerts() {
        QMap< QUrl,FeedUserData > userdatamap(JobManager::instance()->feedManager()->userData());
        QMapIterator< QUrl,FeedUserData > iter(userdatamap);
        while(iter.hasNext()) {
            iter.next();
            QVariantMap key = convertToVariantMap(iter.value());
            if(!key.isEmpty()) {
                alertKeys.append(key);
            }
        }
    }
};

Worker::Worker(JobManagerPrivate* master,QObject *parent) :
    QThread(parent) {
    d = new WorkerPrivate;
    d->master = master;
}

Worker::~Worker() {
    delete d;
}

void Worker::setTask(Worker::TaskType task) {
    d->task = task;
}

Worker::TaskType Worker::task() const {
    return d->task;
}

QList<QVariantMap> Worker::favs() const {
    return d->favs;
}

QList<QVariantMap> Worker::alertKeys() const {
    return d->alertKeys;
}

void Worker::clearData() {
    d->favs.clear();
    d->alertKeys.clear();
}

void Worker::run() {
    d->doTask();
}

// eof

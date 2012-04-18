#include <QDebug>
#include <QMap>
#include <QFile>
#include <QXmlStreamWriter>
#include <QStringListIterator>
#include "JobManagerPrivate.h"
#include "Constants.h"
#include "rssmanager.h"
#include "rssparser.h"
#include "Worker.h"
#include "JobModel.h"
#include "JobAlert.h"
#include "AlertModel.h"
#include "JobInfo.h"

JobManagerPrivate::JobManagerPrivate(QObject* parent)
                  :QObject(parent) {
    searchModel = NULL;
    favoritesModel = NULL;
    newJobsAlertModel = NULL;
    allJobsAlertModel = NULL;
    worker = new Worker(this);
}

JobManagerPrivate::~JobManagerPrivate() {
    saveState();
    delete searchModel;
    delete favoritesModel;
    delete newJobsAlertModel;
    delete allJobsAlertModel;
    qDeleteAll(alerts.begin(),alerts.end());
    worker->wait();
    delete worker;
}

/*!
  Creates a id for the supplied key
  **/
QString JobManagerPrivate::alertId(QVariantMap key) const {
    // create key which is sum of all key params
    return key.value(NJ_SKILL).toString().simplified() +
           key.value(NJ_LOCATION).toString().simplified() +
           key.value(NJ_COUNTRY).toString().simplified();
}

void JobManagerPrivate::restoreState() {
    worker->clearData();
    worker->setTask(Worker::RestoreState);
    connect(worker,SIGNAL(finished()),this,SLOT(handleRestoreCompleted()));
    worker->start();
}

void JobManagerPrivate::handleRestoreCompleted() {
    worker->quit();
    favs = worker->favs();
    alertKeys = worker->alertKeys();
    QListIterator< QVariantMap > iter(alertKeys);
    while(iter.hasNext())
        JobManager::instance()->addAlert(iter.next());
    emit restoreCompleted();
}


void JobManagerPrivate::saveState() {
    RSSManager* rssMgr = JobManager::instance()->feedManager();
    QMapIterator<QString,JobAlert*> iter(alerts);
    qDebug()<<Q_FUNC_INFO<<"saving alerts:"<<alerts.size();
    while(iter.hasNext()) {
        JobAlert* a = iter.next().value();
        if(!a->isVisited())
            rssMgr->forgetCheckpoint(a->model()->baseUrl());
        rssMgr->setUserData(a->model()->baseUrl(),convertToFeedUserData(a->key()));
    }
    saveFavorites();
}

QString JobManagerPrivate::favFilePath() const {
    static QString path;
    if(path.isEmpty())
        path = JobManager::instance()->feedManager()->storagePath() + "/" + FAV_FILENAME;
    return path;
}

// writes to fav file
void JobManagerPrivate::saveFavorites() {
    qDebug()<<Q_FUNC_INFO<<favs.size();
    if(favs.isEmpty())
        return;
    QFile f(favFilePath());
    // TODO: Should we delete this file before using ?
    if(!f.open(QIODevice::WriteOnly)) {
        qWarning()<<Q_FUNC_INFO<<"Unable to open file in write mode: "<<favFilePath();
        return;
    }

    QXmlStreamWriter writer(&f);
    writer.setAutoFormatting(true);
    writer.writeStartDocument();
    writer.writeStartElement(FAV_XML_ROOT);
    QListIterator< QVariantMap > iter(favs);
    while(iter.hasNext()) {
        writer.writeStartElement(FAV_XML_ITEM);
        QVariantMap key = iter.next();
        QMapIterator< QString,QVariant > keyIter(key);
        while(keyIter.hasNext()) {
            keyIter.next();
            writer.writeTextElement(keyIter.key(),keyIter.value().toString());
        }
        writer.writeEndElement(); // closing itemb
    }
    writer.writeEndElement();
    writer.writeEndDocument();
    f.close();
}

void JobManagerPrivate::addToFavorites(QVariantMap key) {
    favs.insert(0,key); // to ensure that latest fav is at top
}

bool JobManagerPrivate::removeFromFavorites(QVariantMap key) {
    int i = favs.indexOf(key);
    if(i >= 0) {
        favs.removeAt(i);
        return true;
    } else {
        return false;
    }
}

FeedUserData JobManagerPrivate::convertToFeedUserData(QVariantMap map) {
    FeedUserData userdata;
    QMapIterator<QString,QVariant> iter(map);
    while(iter.hasNext()) {
        iter.next();
        if(iter.value().canConvert(QVariant::String))
            userdata.insert(iter.key(),iter.value().toString());
    }
    return userdata;
}

RSSManager *JobManagerPrivate::feedManager() {
    return RSSManager::instance(APP_FOLDER_NAME);
}


// eof

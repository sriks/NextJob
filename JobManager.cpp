#include <QMap>
#include <QDebug>
#include "Constants.h"
#include "JobManager.h"
#include "JobModel.h"
#include "JobAlert.h"
#include "rssmanager.h"

struct JobManagerPrivate {
    //QList<JobModel*> models;
    JobModel* jobModel;
    QMap<QString,JobAlert*> alerts;

    JobManagerPrivate() {
        jobModel = NULL;
    }

    QString alertId(QVariantMap key) const {
        return key.value(NJ_SKILL).toString().trimmed() + key.value(NJ_LOCATION).toString().trimmed();
    }

    ~JobManagerPrivate() {
        //qDeleteAll(models.begin(),models.end());
        delete jobModel;
        qDeleteAll(alerts.begin(),alerts.end());
    }
};

JobManager::JobManager(QObject *parent) :
    QObject(parent) {
    d = new JobManagerPrivate;
}

JobManager::~JobManager() {
    delete d;
    delete feedManager();
}

RSSManager *JobManager::feedManager() {
    return RSSManager::instance(APP_FOLDER_NAME);
}

/*!
  Do a free search for the key from network
  **/
void JobManager::search(QVariantMap key) {
    // Check that we are not doing the same search again.
//    JobModel* m = new JobModel(this);
//    d->models.append(m);
//    m->addKey(key);
//    emit searchResultAvailable(m);

    qDebug()<<Q_FUNC_INFO<<key;
    if(!d->jobModel)
        d->jobModel = new JobModel(this);
    d->jobModel->addKey(key);
    emit searchResultAvailable(d->jobModel);
}

/*!
  Adds job alert
  **/
void JobManager::addAlert(QVariantMap key) {
    // TODO: create an alert manager
    // create JobAlert and add to alert map
    QString id = d->alertId(key);
    if(!d->alerts.contains(id)) {
        JobAlert* alert = new JobAlert(key,this);
        d->alerts.insert(id,alert);
        connect(alert,SIGNAL(updateAvailable(QVariantMap,int)),
                this,SIGNAL(alertUpdated(QVariantMap,int)));
        alert->startUpdate();
    } else {
        qWarning()<<Q_FUNC_INFO<<"Alert already added for "<<key;
    }
}

JobAlert *JobManager::alert(QVariantMap key) const {
    return d->alerts.value(d->alertId(key),NULL);
}

void JobManager::fetchAlertUpdates() {
    // TODO: read alerts from storage and fetch updates.

}

// eof

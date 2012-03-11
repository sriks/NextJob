#include <QMap>
#include <QDebug>
#include "Constants.h"
#include "JobManager.h"
#include "JobModel.h"
#include "JobAlert.h"
#include "AlertModel.h"
#include "rssmanager.h"

struct JobManagerPrivate {
    JobModel* searchModel; // used for job search
    AlertModel* newJobsAlertModel; // lists only alerts which has new jobs
    AlertModel* allJobsAlertModel; // lists all alerts
    JobManager* master; // non-owing - do not delete
    QMap<QString,JobAlert*> alerts;

    JobManagerPrivate(JobManager* mgr) {
        master = mgr;
        searchModel = NULL;
        newJobsAlertModel = NULL;
        allJobsAlertModel = NULL;
    }

    ~JobManagerPrivate() {
        saveState();
        delete searchModel;
        delete newJobsAlertModel;
        delete allJobsAlertModel;
        qDeleteAll(alerts.begin(),alerts.end());
    }

    /*!
      Creates a id for the supplied key
      **/
    QString alertId(QVariantMap key) const {
        // create key which is sum of all key params
        return key.value(NJ_SKILL).toString().simplified() +
               key.value(NJ_LOCATION).toString().simplified() +
               key.value(NJ_COUNTRY).toString().simplified();
    }

    void restoreState() {
        QMap< QUrl,FeedUserData > userdatamap(JobManager::feedManager()->userData());
        QMapIterator< QUrl,FeedUserData > iter(userdatamap);
        while(iter.hasNext()) {
            iter.next();
            QVariantMap key = convertToVariantMap(iter.value());
            if(!key.isEmpty())
                master->addAlert(key);
        }
    }

    void saveState() {
        QMapIterator<QString,JobAlert*> iter(alerts);
        while(iter.hasNext()) {
            JobAlert* a = iter.next().value();
            JobManager::feedManager()->setUserData(a->model()->baseUrl(),
                                                   convertToFeedUserData(a->key()));
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

    FeedUserData convertToFeedUserData(QVariantMap map) {
        FeedUserData userdata;
        QMapIterator<QString,QVariant> iter(map);
        while(iter.hasNext()) {
            iter.next();
            if(iter.value().canConvert(QVariant::String))
                userdata.insert(iter.key(),iter.value().toString());
        }
        return userdata;
    }

};

JobManager::JobManager(QObject *parent) :
    QObject(parent) {
    d = new JobManagerPrivate(this);
    d->restoreState();
}

JobManager::~JobManager() {
    delete d;
    delete feedManager();
}

JobManager *JobManager::instance() {
    static JobManager* mgr = NULL;
    if(!mgr)
        mgr = new JobManager;
    return mgr;
}

/*!
  Returns feed manager which can be used across the app.
  **/
RSSManager *JobManager::feedManager() {
    return RSSManager::instance(APP_FOLDER_NAME);
}

/*!
  Do a search for the key from network
  **/
JobModel* JobManager::search(QVariantMap key) {
    // TODO: Check that we are not doing the same search again.
    if(!d->searchModel)
        d->searchModel = new JobModel(JobModel::Search,this);
    d->searchModel->addKey(key);
    emit searchResultAvailable(d->searchModel);
    return d->searchModel;
}

/*!
  Adds job alert
  **/
void JobManager::addAlert(QVariantMap key) {
    JobAlert* a = alert(key);
    if(a == NULL) {
        a = new JobAlert(key,this);
        QString id = d->alertId(key);
        d->alerts.insert(id,a);
        connect(a,SIGNAL(updateAvailable(int,QVariantMap)),
                this,SIGNAL(alertUpdated(int,QVariantMap)));
        emit alertAdded(key);
        emit alertsCountChanged();
    }
    a->update();
}

/*!
  Removes an alert identified by \a key
  **/
void JobManager::removeAlert(QVariantMap key) {
    QString id = d->alertId(key);
    if(d->alerts.contains(id)) {
        JobAlert* a = d->alerts.take(id);
        feedManager()->remove(a->model()->baseUrl());
        delete a;
        emit alertRemoved(key);
        emit alertsCountChanged();
    } else
        qWarning()<<Q_FUNC_INFO<<"Cannot remove alert.\t"<<key;
}

void JobManager::removeAllAlerts() {
    QMapIterator<QString,JobAlert*> iter(d->alerts);
    while(iter.hasNext())
        removeAlert(iter.next().value()->key());
}

void JobManager::updateAlert(QVariantMap key) {
    JobAlert* a = alert(key);
    if(a)
        a->update();
    else
        qWarning()<<Q_FUNC_INFO<<"Cannot update alert which is not added, "<<key;
}

/*!
  Returns job alert identified with \a key
  **/
JobAlert* JobManager::alert(QVariantMap key) const {
    return d->alerts.value(d->alertId(key),NULL);
}

QList<JobAlert*> JobManager::alerts() const {
    return d->alerts.values();
}

JobAlert *JobManager::alert(int index) const {
    return alerts().at(index);
}

int JobManager::alertsCount() const {
    return d->alerts.size();
}

/*!
  Updates all alerts
  **/
void JobManager::updateAllAlerts() {
    QMapIterator<QString,JobAlert*> iter(d->alerts);
    while(iter.hasNext())
        iter.next().value()->update();
}

AlertModel *JobManager::newJobsAlertModel() {
    if(!d->newJobsAlertModel) {
        d->newJobsAlertModel = new AlertModel(AlertModel::AlertsWithNewJobs,this);
        connect(this,SIGNAL(alertAdded(QVariantMap)),d->newJobsAlertModel,SLOT(alertAdded(QVariantMap)));
        connect(this,SIGNAL(alertRemoved(QVariantMap)),d->newJobsAlertModel,SLOT(alertRemoved(QVariantMap)));
        connect(this,SIGNAL(alertUpdated(int,QVariantMap)),d->newJobsAlertModel,SLOT(alertUpdated(int,QVariantMap)));
    }
    return d->newJobsAlertModel;
}

AlertModel *JobManager::allAlertsModel() {
    if(!d->allJobsAlertModel) {
        d->allJobsAlertModel = new AlertModel(AlertModel::AllAlerts,this);
        connect(this,SIGNAL(alertAdded(QVariantMap)),d->allJobsAlertModel,SLOT(alertAdded(QVariantMap)));
        connect(this,SIGNAL(alertRemoved(QVariantMap)),d->allJobsAlertModel,SLOT(alertRemoved(QVariantMap)));
        connect(this,SIGNAL(alertUpdated(int,QVariantMap)),d->allJobsAlertModel,SLOT(alertUpdated(int,QVariantMap)));
    }
    return d->allJobsAlertModel;
}

// eof

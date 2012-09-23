#include <QMap>
#include <QDebug>
#include <QStringListIterator>
#include "Constants.h"
#include "JobManager.h"
#include "JobModel.h"
#include "JobAlert.h"
#include "AlertModel.h"
#include "JobInfo.h"
#include "rssmanager.h"
#ifdef DC_HARMATTAN
#include "ShareUi.h"
#endif
#include "JobManagerPrivate.h"

JobManager::JobManager(QObject *parent) :
    QObject(parent) {
    d = new JobManagerPrivate(this);
    connect(d,SIGNAL(restoreCompleted()),this,SIGNAL(initializationCompleted()));
    connect(d,SIGNAL(restoreCompleted()),this,SLOT(loadAlerts()));
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
  Initializes jobmanager and loads with required data.
  JobManager can be used after the signal initializationCompleted()
  **/
void JobManager::initialize() {
    // TODO: Fix it.
    // All data is parsed and prepared in worker thread which uses rssmanager.
    // If rssmanager is created in worker thread, then it has qobject parent problem when used in main thread which has to be fixed.
    // Hence creating it in main thread so that it resides here.
    // But instantiating rssmanger itself introduces some delay which can be lived through for now.

    RSSManager* m = instance()->feedManager();
    Q_UNUSED(m);
    d->restoreState();
    // initializationCompleted is emitted when restore is completed.
}

/*!
  Returns feed manager which can be used across the app.
  **/
RSSManager *JobManager::feedManager() {
    return d->feedManager();
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
    if(isAlertAdded(key))
       return;
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

/*!
 * \brief JobManager::newJobsAlertModel
 *  Returns alert model for jobs that have new updates.
 * \return
 */
// TODO: Change method name
AlertModel *JobManager::newJobsAlertModel() {
    if(!d->newJobsAlertModel) {
        d->newJobsAlertModel = new AlertModel(AlertModel::AlertsWithNewJobs,this);
        connect(this,SIGNAL(alertAdded(QVariantMap)),d->newJobsAlertModel,SLOT(alertAdded(QVariantMap)));
        connect(this,SIGNAL(alertRemoved(QVariantMap)),d->newJobsAlertModel,SLOT(alertRemoved(QVariantMap)));
        connect(this,SIGNAL(alertUpdated(int,QVariantMap)),d->newJobsAlertModel,SLOT(alertUpdated(int,QVariantMap)));
    }
    return d->newJobsAlertModel;
}

/*!
 * \brief JobManager::allAlertsModel
 * Returns alert model for all jobs irrespective whether there are any updates or not.
 * \return AlertModel
 */
// TODO: Change method name
AlertModel *JobManager::allAlertsModel() {
    if(!d->allJobsAlertModel) {
        d->allJobsAlertModel = new AlertModel(AlertModel::AllAlerts,this);
        connect(this,SIGNAL(alertAdded(QVariantMap)),d->allJobsAlertModel,SLOT(alertAdded(QVariantMap)));
        connect(this,SIGNAL(alertRemoved(QVariantMap)),d->allJobsAlertModel,SLOT(alertRemoved(QVariantMap)));
        connect(this,SIGNAL(alertUpdated(int,QVariantMap)),d->allJobsAlertModel,SLOT(alertUpdated(int,QVariantMap)));
    }
    return d->allJobsAlertModel;
}

bool JobManager::isAlertAdded(QVariantMap key) const {
    return d->alerts.contains(d->alertId(key));
}

JobModel *JobManager::favoriteJobs() {
    if(!d->favoritesModel) {
        d->favoritesModel = new JobModel(JobModel::Favorites,this);
        connect(this,SIGNAL(favoriteAdded(QVariantMap)),
                d->favoritesModel,SLOT(reset()));
        connect(this,SIGNAL(favoriteRemoved(QVariantMap)),
                d->favoritesModel,SLOT(reset()));
    }
    d->favoritesModel->reset(); // load data
    return d->favoritesModel;
}

void JobManager::addToFavorites(QVariantMap key) {
    if(!isFavorite(key)) {
        d->addToFavorites(key);

        emit favoriteAdded(key);
    }
}

void JobManager::removeFromFavorites(QVariantMap key) {
    if(d->removeFromFavorites(key))
        emit favoriteRemoved(key);
}

void JobManager::removeAllFavorites() {
    d->removeAllFavorites();
    emit allFavoritesRemoved();
}

int JobManager::favoritesCount() const {
    return d->favs.size();
}

QVariantMap JobManager::favoriteKey(int index) const {
    return d->favs.at(index);
}

bool JobManager::isFavorite(QVariantMap key) const {
    QString favLookupKey = key.value(NJ_FAVS_LOOKUP_KEY).toString().trimmed();
    return d->favLookup.contains(favLookupKey);
}

void JobManager::share(QVariantMap key) {
#ifdef DC_HARMATTAN // Including any of Dreamcode sources should define this macro.
    ShareUi s;
    s.share(key.value(NJ_PROP_KEY_TITLE).toString(),"",key.value(NJ_PROP_KEY_URL).toString());
#endif
}

// eof

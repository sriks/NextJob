#include <QMap>
#include <QDebug>
#include <QFile>
#include <QXmlStreamWriter>
#include <QStringListIterator>
#include "Constants.h"
#include "JobManager.h"
#include "JobModel.h"
#include "JobAlert.h"
#include "AlertModel.h"
#include "JobInfo.h"
#include "rssmanager.h"

const QString FAV_FILENAME("njfavs.xml");
const QString FAV_XML_ROOT("favs");
const QString FAV_XML_ITEM("item");

struct JobManagerPrivate {
    JobModel* searchModel; // used for job search
    JobModel* favoritesModel; // used for favorite jobs
    AlertModel* newJobsAlertModel; // lists only alerts which has new jobs
    AlertModel* allJobsAlertModel; // lists all alerts
    JobManager* master; // non-owing - do not delete
    QMap<QString,JobAlert*> alerts;
    QList< QVariantMap > favs;
    QStringList jobInfoKeys;

    JobManagerPrivate(JobManager* mgr) {
        master = mgr;
        searchModel = NULL;
        favoritesModel = NULL;
        newJobsAlertModel = NULL;
        allJobsAlertModel = NULL;
        jobInfoKeys << NJ_PROP_KEY_TITLE << NJ_PROP_KEY_EMPNAME << NJ_PROP_KEY_LOCATION << NJ_PROP_KEY_DESCRIPTION
                    << NJ_PROP_KEY_URL << NJ_PROP_KEY_DATE << NJ_PROP_KEY_SOURCE;
    }

    ~JobManagerPrivate() {
        saveState();
        delete searchModel;
        delete favoritesModel;
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

    QString favId(QVariantMap key) const {
        return key.value(NJ_PROP_KEY_TITLE).toString().simplified() +
               key.value(NJ_PROP_KEY_EMPNAME).toString().simplified() +
               key.value(NJ_PROP_KEY_LOCATION).toString().simplified() +
               key.value(NJ_PROP_KEY_URL).toString().simplified();
    }

    void restoreState() {
        restoreAlerts();
    }

    void restoreAlerts() {
        // alerts
        QMap< QUrl,FeedUserData > userdatamap(JobManager::feedManager()->userData());
        QMapIterator< QUrl,FeedUserData > iter(userdatamap);
        while(iter.hasNext()) {
            iter.next();
            QVariantMap key = convertToVariantMap(iter.value());
            if(!key.isEmpty())
                master->addAlert(key);
        }
    }

    void restoreFavs() {
        // favorites
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
        }
        f.close();
    }

    void saveState() {
        RSSManager* rssMgr = JobManager::feedManager();
        QMapIterator<QString,JobAlert*> iter(alerts);
        while(iter.hasNext()) {
            JobAlert* a = iter.next().value();
            if(!a->isVisited())
                rssMgr->forgetCheckpoint(a->model()->baseUrl());
            rssMgr->setUserData(a->model()->baseUrl(),convertToFeedUserData(a->key()));
        }
    }

    QString favFilePath() const {
        static QString path;
        if(path.isEmpty())
            path = JobManager::feedManager()->storagePath() + "/" + FAV_FILENAME;
        return path;
    }

    // writes to fav file
    void saveFavorites() {
        if(favs.isEmpty())
            return;
        QFile f(favFilePath());
        // TODO: Should we delete this file before using.
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

    void addToFavorites(QVariantMap key) {
        favs.insert(0,key); // to ensure that latest fav is at top
    }

    bool removeFromFavorites(QVariantMap key) {
        int i = favs.indexOf(key);
        if(i >= 0) {
            favs.removeAt(i);
            return true;
        } else {
            return false;
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
    d->restoreState();
    emit initializationCompleted();
    d->restoreFavs();
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

JobModel *JobManager::favoriteJobs() {
    d->saveFavorites();
    if(!d->favoritesModel) {
        d->restoreFavs();
        d->favoritesModel = new JobModel(JobModel::Favorites,this);
        connect(this,SIGNAL(favoriteAdded(QVariantMap)),
                d->favoritesModel,SLOT(reset()));
        connect(this,SIGNAL(favoriteRemoved(QVariantMap)),
                d->favoritesModel,SLOT(reset()));
        QString favPath = feedManager()->storagePath() + "/" + FAV_FILENAME;
        QVariantMap key;
        key.insert(NJ_FAV_PATH,QVariant(favPath));
        d->favoritesModel->addKey(key);
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

int JobManager::favoritesCount() const {
    return d->favs.size();
}

QVariantMap JobManager::favoriteKey(int index) const {
    return d->favs.at(index);
}

bool JobManager::isFavorite(QVariantMap key) const {
    return d->favs.contains(key);
}


// eof

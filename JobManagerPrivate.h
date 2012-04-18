#ifndef JOBMANAGERPRIVATE_H
#define JOBMANAGERPRIVATE_H

#include <QObject>
#include <QVariantMap>
#include "JobManager.h"
#include "rssmanager.h"

class Worker;
class JobManagerPrivate: public QObject {
    Q_OBJECT

public:
    JobManagerPrivate(QObject* parent);
    ~JobManagerPrivate();
    QString alertId(QVariantMap key) const;
    void restoreState();
    void saveState();
    QString favFilePath() const;
    void saveFavorites();
    void addToFavorites(QVariantMap key);
    bool removeFromFavorites(QVariantMap key);
    FeedUserData convertToFeedUserData(QVariantMap map);
    RSSManager* feedManager();
public slots:
    void handleRestoreCompleted();

signals:
    void restoreCompleted();

public:
    JobModel* searchModel; // used for job search
    JobModel* favoritesModel; // used for favorite jobs
    AlertModel* newJobsAlertModel; // lists only alerts which has new jobs
    AlertModel* allJobsAlertModel; // lists all alerts
    Worker* worker; // worker thread performing intial loading
    QMap<QString,JobAlert*> alerts;
    QList< QVariantMap > alertKeys; // loaded by worker thread
    QList< QVariantMap > favs;
};

#endif // JOBMANAGERPRIVATE_H

#ifndef JOBMANAGER_H
#define JOBMANAGER_H

#include <QObject>
#include <QVariantMap>

class JobManagerPrivate;
class JobModel;
class JobAlert;
class AlertModel;
class JobInfo;
class RSSManager;
class JobManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int alertsCount READ alertsCount NOTIFY alertsCountChanged)

public:
    ~JobManager();
    static JobManager* instance();
    static RSSManager* feedManager();
protected:
    explicit JobManager(QObject *parent = 0);
signals:
    void initializationCompleted();
    void searchResultAvailable(JobModel* jobModel);
    void alertAdded(QVariantMap key);
    void alertRemoved(QVariantMap key);
    void alertUpdated(int newItemsCount,QVariantMap source);
    void alertsCountChanged();
    void favoriteAdded(QVariantMap key);
    void favoriteRemoved(QVariantMap key);

public slots:
    void initialize();
    JobModel* search(QVariantMap key);

    // Alerts
    void addAlert(QVariantMap key);
    void removeAlert(QVariantMap key);
    void removeAllAlerts();
    void updateAlert(QVariantMap key);
    JobAlert* alert(QVariantMap key) const;
    QList<JobAlert*> alerts() const;
    JobAlert* alert(int index) const;
    int alertsCount() const;
    void updateAllAlerts();
    AlertModel* newJobsAlertModel();
    AlertModel* allAlertsModel();
    bool isAlertAdded(QVariantMap key) const;

    // Favorites
    JobModel* favoriteJobs();
    void addToFavorites(QVariantMap key);
    void removeFromFavorites(QVariantMap key);
    int favoritesCount() const;
    QVariantMap favoriteKey(int index) const;
    bool isFavorite(QVariantMap key) const;

    // Share
    void share(QVariantMap key);

private:
    JobManagerPrivate* d;
};

#endif // JOBMANAGER_H

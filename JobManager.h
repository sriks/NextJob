#ifndef JOBMANAGER_H
#define JOBMANAGER_H

#include <QObject>
#include <QVariantMap>

class JobManagerPrivate;
class JobModel;
class JobAlert;
class AlertModel;
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
    void searchResultAvailable(JobModel* jobModel);
    void alertAdded(QVariantMap key);
    void alertRemoved(QVariantMap key);
    void alertUpdated(int newItemsCount,QVariantMap source);
    void alertsCountChanged();

public slots:
    JobModel* search(QVariantMap key);
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

private:
    JobManagerPrivate* d;
};

#endif // JOBMANAGER_H

#ifndef JOBMANAGER_H
#define JOBMANAGER_H

#include <QObject>
#include <QVariantMap>

class JobManagerPrivate;
class JobModel;
class JobAlert;
class RSSManager;
class JobManager : public QObject
{
    Q_OBJECT
public:
    explicit JobManager(QObject *parent = 0);
    ~JobManager();
    static RSSManager* feedManager();
signals:
    void searchResultAvailable(JobModel* jobModel);
    void alertUpdated(QVariantMap key,int newItemsCount);

public slots:
    void search(QVariantMap key);
    void addAlert(QVariantMap key);
    JobAlert* alert(QVariantMap key) const;
    void fetchAlertUpdates();

private:
    JobManagerPrivate* d;
};

#endif // JOBMANAGER_H

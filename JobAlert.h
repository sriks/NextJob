#ifndef JOBALERT_H
#define JOBALERT_H

#include <QObject>
#include <QVariantMap>

class JobModel;
class JobAlertPrivate;
class JobAlert : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int newJobsCount READ newJobsCount NOTIFY newJobsCountChanged)
public:
    explicit JobAlert(QVariantMap key,QObject *parent = 0);
    ~JobAlert();

signals:
    void updateAvailable(int newItemsCount,QVariantMap key);
    void newJobsCountChanged();

public slots:
    QVariantMap key() const;
    JobModel* model() const;
    QString displayName() const;
    QString displayNameWithJobCount() const;
    QString skill() const;
    QString location() const;
    QString country() const;
    int newJobsCount() const;
    void update();
    void onUpdateAvailable(int newJobsCount,QVariantMap key);
    void setVisited(bool val);
    bool isVisited() const;
private:
    JobAlertPrivate* d;
};

#endif // JOBALERT_H

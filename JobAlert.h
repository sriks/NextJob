#ifndef JOBALERT_H
#define JOBALERT_H

#include <QObject>
#include <QVariantMap>

class JobModel;
class JobAlertPrivate;
class JobAlert : public QObject
{
    Q_OBJECT
public:
    explicit JobAlert(QVariantMap key,QObject *parent = 0);
    ~JobAlert();
signals:
    void updateAvailable(QVariantMap key,int newItemsCount);
public slots:
    QVariantMap key() const;
    JobModel* model() const;
    int newItemsCount() const;
    void startUpdate();
    void onUpdateAvailable(int newItemsCount);

private:
    JobAlertPrivate* d;
};

#endif // JOBALERT_H

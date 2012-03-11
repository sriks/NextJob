#ifndef ALERTMODEL_H
#define ALERTMODEL_H

#include <QAbstractListModel>

class AlertModelPrivate;
class AlertModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum AlertModelRoles {
        AlertInfo = Qt::UserRole + 1
    };

    enum AlertModelType {
        AllAlerts,
        AlertsWithNewJobs
    };

    explicit AlertModel(AlertModelType type, QObject *parent = 0);
    ~AlertModel();
    AlertModelType type() const;

signals:

public slots:
    // from QAbstractListModel
    virtual int rowCount(const QModelIndex &parent) const;
    virtual QVariant data(const QModelIndex &index, int role) const;

    void loadData();
    void alertAdded(QVariantMap source);
    void alertRemoved(QVariantMap source);
    void alertUpdated(int newItemsCount,QVariantMap source);
    int count() const;
private:
    AlertModelPrivate* d;
};

#endif // ALERTMODEL_H

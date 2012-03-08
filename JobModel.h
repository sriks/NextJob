#ifndef JOBMODEL_H
#define JOBMODEL_H

#include <QAbstractListModel>

class JobModelPrivate;
class JobModel : public QAbstractListModel
{
    Q_OBJECT
    enum JobModelRoles {
        JobInfo = Qt::UserRole + 1
    };
public:
    explicit JobModel(QObject *parent = 0);
    ~JobModel();

    // from QAbstractListModel
    virtual int rowCount(const QModelIndex &parent) const;
    virtual QVariant data(const QModelIndex &index, int role) const;

signals:
    void networkRequestStarted();
    void networkRequestFinished();
    void error(QString errorMessage);
    void updateAvailable(int newItemsCount);
    void noUpdateAvailable();
    void dataFinished();

public slots:
    int count() const;
    void addKey(QVariantMap key);
    void next();

private:
    friend class JobModelPrivate;
    JobModelPrivate* d;
};

#endif // JOBMODEL_H

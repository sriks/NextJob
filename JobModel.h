#ifndef JOBMODEL_H
#define JOBMODEL_H

#include <QAbstractListModel>
#include <QUrl>

class JobModelPrivate;
class JobModel : public QAbstractListModel
{
    Q_OBJECT
    enum JobModelRoles {
        JobInfo = Qt::UserRole + 1
    };

public:
    /*!
      Job model type
      **/
    enum JobModelType {
        Search, // General search, updates all items
        Alert,   // For using with job alert, updates if any new items are found.
        Favorites // Used to list favorite jobs
    };

    explicit JobModel(JobModelType type,QObject *parent = 0);
    ~JobModel();

    // from QAbstractListModel
    virtual int rowCount(const QModelIndex &parent) const;
    virtual QVariant data(const QModelIndex &index, int role) const;

signals:
    void networkRequestStarted();
    void networkRequestFinished();
    void error(QString errorMessage);
    void updateAvailable(int newItemsCount,QVariantMap source);
    void noDataAvailable();
    void dataFinished();

public slots:
    int count() const;
    void addKey(QVariantMap key);
    QVariantMap key() const;
    QUrl baseUrl() const;
    void next();
    void reset();

private:
    friend class JobModelPrivate;
    JobModelPrivate* d;
};

#endif // JOBMODEL_H

#ifndef JOBMODELPRIVATE_H
#define JOBMODELPRIVATE_H

#include <QObject>
#include <QUrl>
#include <QVariantMap>
#include "JobModel.h"

class JobInfo;
class RSSManager;
class RSSParser;
class JobModelPrivate : public QObject
{
    Q_OBJECT
public:
    explicit JobModelPrivate(JobModel* qPtr,JobModel::JobModelType type,QObject *parent = 0);
    ~JobModelPrivate();
    int count() const;
    RSSManager* feedMgr() const;
    RSSParser* currentParser();
    QVariantMap key() const;
public slots:
#if defined(TEST_WITH_DUMMY_DATA)
    void sendDummyData();
#endif
    void add(QVariantMap key);
    void updateAvailable(QUrl url, int newItemsCount);
    void error(QString,QUrl);
    JobInfo* at(int index);
    void fetchMoreData();
    void sendRequest(QUrl url);
    void resetModel();
    void append(int from,int count);
    void cleanup();
protected:
    QVariantMap parseForInfo(int index);

private:
    friend class JobModel;
    JobModel* d;

    JobModel* q; // public ptr non-owning
    RSSParser* mCurrentParser;
    const JobModel::JobModelType mJobModelType;
    QVariantMap mKey; // key based on which data is fetched
    QString mBaseUrl; // url formed from supplied data
    QUrl mCurrentUrl; // Current url request
    QList<JobInfo*> mData;
    QList<QUrl> mAdditionalUrls; // urls constructed to fetch more data other than base url
    int mCount;
    int mDataOffset;
    bool mDataFinished; // indicates that no more data is available to fetch from.
    bool mInvalidateCurrentParser;
};

#endif // JOBMODELPRIVATE_H

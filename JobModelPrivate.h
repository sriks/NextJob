#ifndef JOBMODELPRIVATE_H
#define JOBMODELPRIVATE_H

//#define TEST_WITH_DUMMY_DATA

#include <QObject>
#include <QUrl>
#include <QVariantMap>

class JobInfo;
class RSSManager;
class JobModel;
class RSSParser;
class JobModelPrivate : public QObject
{
    Q_OBJECT
public:
    explicit JobModelPrivate(JobModel* qPtr,QObject *parent = 0);
    ~JobModelPrivate();
    int count() const;
    RSSManager* feedMgr() const;
    RSSParser* currentParser();
public slots:
    void add(QVariantMap key);
    void updateAvailable(QUrl url, int newItemsCount);
    void error(QString,QUrl);
    JobInfo* at(int index);
    void fetchMoreData();
    void sendRequest(QUrl url);
#if defined(TEST_WITH_DUMMY_DATA)
    void sendDummyData();
#endif
protected slots:
    void resetModel();
    void append(int from,int count);
    void cleanup();
protected:
    QVariantMap parseForInfo(int index);

private:
    JobModel* q; // public ptr non-owning
    RSSParser* mCurrentParser;
    QString mBaseUrl; // url formed from supplied data
    QUrl mCurrentUrl; // Current url request
    QList<JobInfo*> mData;
    int mCount;
    int mDataOffset;
    bool mDataFinished; // indicates that no more data is available to fetch from.
    bool mInvalidateCurrentParser;
};

#endif // JOBMODELPRIVATE_H

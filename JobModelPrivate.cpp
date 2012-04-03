#if defined(TEST_WITH_DUMMY_DATA)
#include <QTimer>
#include <QDate>
#endif
#include <QDebug>
#include <QFile>
#include "JobModelPrivate.h"
#include "rssmanager.h"
#include "rssparser.h"
#include "Constants.h"
#include "JobInfo.h"
#include "JobModel.h"
#include "JobManager.h"

const int MAX_ITEMS_PER_REQ = 20;
const int MAX_ITEMS         = 60;

// TODO: delegate construction of server url so that we can use it with
// any server that supports rss output.
const QString NJ_SERVER_URL = "http://www.indeed.co.in/rss";
const QString NJ_SERVER_QUERY_SKILL = "q";
const QString NJ_SERVER_QUERY_LOC = "l";
const QString NJ_SERVER_QUERY_START = "start";

JobModelPrivate::JobModelPrivate(JobModel* qPtr,JobModel::JobModelType type,QObject *parent) :
                 QObject(parent),
                 mJobModelType(type),
                 mDataOffset(0),
                 mCount(0),
                 mDataFinished(false),
                 mInvalidateCurrentParser(true),
                 mCurrentParser(NULL),
                 q(qPtr) {

    connect(feedMgr(),SIGNAL(updateAvailable(QUrl,int)),
            this,SLOT(updateAvailable(QUrl,int)),
            Qt::UniqueConnection);

    connect(feedMgr(),SIGNAL(error(QString,QUrl)),
            this,SLOT(error(QString,QUrl)),
            Qt::UniqueConnection);
}

JobModelPrivate::~JobModelPrivate() {
    cleanup();
}

int JobModelPrivate::count() const {
    return mCount;
}

/*!
  Convinience method to get feed manager
  **/
RSSManager *JobModelPrivate::feedMgr() const {
    return JobManager::feedManager();
}

/*!
  Returns parser that is setup to parse current feed response.
  \note Do not delete this returning pointer.
  **/
RSSParser *JobModelPrivate::currentParser() {
    if(mInvalidateCurrentParser) {
        if(mCurrentParser)
            delete mCurrentParser;
        mCurrentParser = feedMgr()->parser(mCurrentUrl);
        mInvalidateCurrentParser = false;
    }
    return mCurrentParser;
}

QVariantMap JobModelPrivate::key() const {
    return mKey;
}

/*!
  Adds key to fetch data
  **/
void JobModelPrivate::add(QVariantMap key) {
    cleanup();
    mKey = key;
    QString skill,loc;
    if(key.size()) {
        skill = key.value(NJ_SKILL,QVariant()).toString();
        loc   = key.value(NJ_LOCATION,QVariant()).toString();
    }
    // Construct base url
    // TODO: Make this to be generic
    if(!skill.isEmpty()) {
        mBaseUrl.setUrl(NJ_SERVER_URL);
         mBaseUrl.addQueryItem(NJ_SERVER_QUERY_SKILL,skill);
         if(!loc.isEmpty())
            mBaseUrl.addQueryItem(NJ_SERVER_QUERY_LOC,loc);
    }
    sendRequest(mBaseUrl);
}

/*!
  Cleans up internal representation and this object can be used
  to fecth data from a new key.
  **/
void JobModelPrivate::cleanup() {
    qDeleteAll(mData.begin(),mData.end());
    mData.clear();
    mDataFinished = false;
    mDataOffset = 0;
    mCount = 0;
    mBaseUrl = QString();
    mCurrentUrl = QUrl();
    if(mCurrentParser)
        delete mCurrentParser;
    mCurrentParser = NULL;
    mInvalidateCurrentParser = true;
    mKey.clear();
    QListIterator<QUrl> iter(mAdditionalUrls);
    while(iter.hasNext())
        feedMgr()->remove(iter.next());
    mAdditionalUrls.clear();
}


void JobModelPrivate::fetchMoreData() {
#if defined(TEST_WITH_DUMMY_DATA)
    // inject some delay to simulate a typical network request
    QTimer::singleShot(4000,this,SLOT(sendDummyData()));
    return;
#endif
    if(JobModel::Favorites == mJobModelType)
        return;

    if(!mDataFinished && mCount >= MAX_ITEMS_PER_REQ && mCount <= MAX_ITEMS) {
        int nextStart = (MAX_ITEMS_PER_REQ*mDataOffset)+1;
        QUrl url(mBaseUrl);
        url.addQueryItem(NJ_SERVER_QUERY_START,QString().setNum(nextStart));
        // use resolved to correctly form a url of what we mean.
        url = mBaseUrl.resolved(url);
        mAdditionalUrls.append(url); // these are removed from feedmgr with cleanup
        sendRequest(url);
    } else {
        qDebug()<<Q_FUNC_INFO<<"Max items reached.."<<mCount;
        mDataFinished = true;
        emit q->dataFinished();
    }
}

void JobModelPrivate::sendRequest(QUrl url) {
    qDebug()<<Q_FUNC_INFO<<url;
#if defined(TEST_WITH_DUMMY_DATA)
    updateAvailable(url,MAX_ITEMS_PER_REQ);
#else
    mCurrentUrl = url; // Conviniently updating it here
    feedMgr()->add(url);
    feedMgr()->update(url);
    emit q->networkRequestStarted();
#endif
}

#if defined(TEST_WITH_DUMMY_DATA)
void JobModelPrivate::sendDummyData() {
    updateAvailable(QUrl(),MAX_ITEMS_PER_REQ);
}
#endif

void JobModelPrivate::updateAvailable(QUrl url, int newItemsCount) {
    // Check if this update belongs to us
    if(url != mCurrentUrl)
        return;
    qDebug()<<Q_FUNC_INFO<<"url :"<<newItemsCount;
    mInvalidateCurrentParser = true;
    emit q->networkRequestFinished();
    RSSParser* p = currentParser();
#if defined(TEST_WITH_DUMMY_DATA)
    if(1) { // make it success always
#else
    if(!mDataFinished && p && p->count()) {
#endif
        int items = 0;
        if(JobModel::Search == mJobModelType) {
#if defined(TEST_WITH_DUMMY_DATA)
            items = MAX_ITEMS_PER_REQ;
#else
            items =  p->count();
#endif
            // Let us handle only fixed number of items per dataset.
            if(items > MAX_ITEMS_PER_REQ)
                items = MAX_ITEMS_PER_REQ;

        } else if(JobModel::Alert == mJobModelType) {
            items = newItemsCount;
        }

        int prevCount = mCount;
        mCount += items;
        if(mCount == items)
            resetModel();
        else if (mCount > items)
            append(prevCount,mCount);

        emit q->updateAvailable(newItemsCount,mKey);

        if(mCount < MAX_ITEMS && items >= MAX_ITEMS_PER_REQ)
            mDataOffset++;
        else
            mDataFinished = true;

    } else {
        qWarning()<<Q_FUNC_INFO<<"No items available for "<<url;
        emit q->noDataAvailable();
    }
}

void JobModelPrivate::error(QString errMsg,QUrl url) {
    qWarning()<<Q_FUNC_INFO<<errMsg<<"\n"<<url;
    emit q->networkRequestFinished();
    emit q->error(errMsg);
}

JobInfo *JobModelPrivate::at(int index) {
    JobInfo* info = NULL;
    if(index >=0 && index < mData.size()) {
        info = mData.at(index);
    } else {
        // Parse info and create a jobinfo.
        // Item range checking does not apply for local storage data.
        int i = (index >= MAX_ITEMS_PER_REQ)?(index%MAX_ITEMS_PER_REQ):(index);
        QVariantMap key = parseForInfo(i);
        info = new JobInfo(key,this);
        mData.insert(index,info);
    }
    return info;
}

QVariantMap JobModelPrivate::parseForInfo(int index) {
#if defined(TEST_WITH_DUMMY_DATA)
    // Return dummy data
    QVariantMap info;
    info.insert(NJ_PROP_KEY_TITLE,QString("Qt Developer ")+QString().setNum((mDataOffset-1)*MAX_ITEMS_PER_REQ + index));
    info.insert(NJ_PROP_KEY_EMPNAME,QString("Dreamcode Dreamcode Dreamcode Dreamcode Dreamcode Dreamcode"));
    info.insert(NJ_PROP_KEY_LOCATION,QString("Hyderabad"));
    info.insert(NJ_PROP_KEY_DATE,QDate::currentDate().toString());
    QString testDesc = "Strong Experience in building multithreaded QT applicationsStrong knowledge in multithreaded programming.</br>Strong object oriented programming skills.Strong Experience in debugging and analysis skills to identify memory leaks and dead locks.Expertise in VS2008/10 IDE.";
    info.insert(NJ_PROP_KEY_DESCRIPTION,testDesc.simplified());
    info.insert(NJ_PROP_KEY_URL,QUrl("http://india.indeed.com/viewjob?t=Software+Engineer&amp;c=NOKIA&amp;l=Bangalore%2C+Karnataka&amp;jk=5a0970bb270c2bd0"));
    return info;
#else
    //qDebug()<<Q_FUNC_INFO<<index;
    if(JobModel::Favorites == mJobModelType && index <= JobManager::instance()->favoritesCount())
        return JobManager::instance()->favoriteKey(index);

    RSSParser* parser = currentParser();
    QVariantMap info;
    if(parser && index < parser->count()) {
        QString title;
        QString emp;
        QString loc;

        if(JobModel::Favorites == mJobModelType) {
            // favorites are saved in proper format.
            title = parser->itemElement(index,NJ_PROP_KEY_TITLE);
            emp = parser->itemElement(index,NJ_PROP_KEY_EMPNAME);
            loc = parser->itemElement(index,NJ_PROP_KEY_LOCATION);
        } else {
            QString t = parser->decodeHtml(parser->itemElement(index,NJ_PROP_KEY_TITLE));
            t = t.simplified();
            //Expected title format: CAE, II - Synopsys -  Hyderabad, Andhra Pradesh
            QStringList tmp = t.split(" - ");
            if(tmp.size() >= 2) {
                title = tmp.first();
                emp = tmp.at(tmp.size() - 2); // 2nd from last
                loc = tmp.last();
            } else {
                title = emp = loc = tmp.first();
            }
        }
        info.insert(NJ_PROP_KEY_TITLE,title.trimmed());
        info.insert(NJ_PROP_KEY_EMPNAME,emp.trimmed());
        info.insert(NJ_PROP_KEY_LOCATION,loc.trimmed());
        QString d= parser->decodeHtml(parser->itemElement(index,NJ_PROP_KEY_DESCRIPTION));
        info.insert(NJ_PROP_KEY_DESCRIPTION,d.simplified()); // Having some issue with this formatted text, hence simplifying.
        info.insert(NJ_PROP_KEY_URL,parser->itemElement(index,NJ_PROP_KEY_URL));
        info.insert(NJ_PROP_KEY_DATE,parser->itemElement(index,NJ_PROP_KEY_DATE).trimmed());
        info.insert(NJ_PROP_KEY_SOURCE,parser->itemElement(index,NJ_PROP_KEY_SOURCE).trimmed());
    } else {
        qDebug()<<Q_FUNC_INFO<<"Ignoring req for "<<index;
    }
return info;
#endif
}

void JobModelPrivate::resetModel() {
    // Since data source is local storage we can handle the logic here itself.
    if(JobModel::Favorites == mJobModelType) {
        cleanup();
        mCount = JobManager::instance()->favoritesCount();
    }
    q->beginResetModel();
    q->endResetModel();
}

void JobModelPrivate::append(int from,int count) {
    for(int i=from;i<count;++i) {
        q->beginInsertRows(QModelIndex(),i,i);
        q->endInsertRows();
    }
}

// eof

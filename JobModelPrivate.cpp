#if defined(TEST_WITH_DUMMY_DATA)
#include <QTimer>
#include <QDate>
#endif
#include <QDebug>
#include "JobModelPrivate.h"
#include "rssmanager.h"
#include "rssparser.h"
#include "Constants.h"
#include "JobInfo.h"
#include "JobModel.h"
#include "JobManager.h"

const int MAX_ITEMS_PER_REQ = 20;
const int MAX_ITEMS         = 60;

// delegate construction of server url so that we can use it with
// any server that supports rss output.
// hopefully it can be done in add()
const QString NJ_SERVER_URL = "http://www.indeed.co.in/rss?";

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
    qDebug()<<Q_FUNC_INFO;
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
         mBaseUrl = NJ_SERVER_URL + "q=" + skill;
        if(!loc.isEmpty())
             mBaseUrl += "&l=" + loc;
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
    qDebug()<<Q_FUNC_INFO;
#if defined(TEST_WITH_DUMMY_DATA)
    // inject some delay to simulate a typical network request
    QTimer::singleShot(4000,this,SLOT(sendDummyData()));
    return;
#endif
    if(/*mJobModelType == JobModel::Search &&*/
       !mDataFinished &&
       mCount >= MAX_ITEMS_PER_REQ && mCount <= MAX_ITEMS) {
        int nextStart = (MAX_ITEMS_PER_REQ*mDataOffset)+1;
        QUrl url = mBaseUrl+"&start="+QString().setNum(nextStart);
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
            items =  p->count();
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
            append(prevCount-1,mCount);

        emit q->updateAvailable(newItemsCount,mKey);

        if(items >= MAX_ITEMS_PER_REQ)
            mDataOffset++;
        else
            mDataFinished = true;

    } else {
        qWarning()<<Q_FUNC_INFO<<"No items available for "<<url;
        emit q->noUpdateAvailable();
    }
}

void JobModelPrivate::error(QString errMsg,QUrl url) {
    qWarning()<<Q_FUNC_INFO<<errMsg<<"\n"<<url;
    emit q->networkRequestFinished();
    emit q->error(errMsg);
}

JobInfo *JobModelPrivate::at(int index) {
    qDebug()<<Q_FUNC_INFO<<index;
    JobInfo* info = NULL;
    if(index >=0 && index < mData.size()) {
        info = mData.at(index);
    } else {
        // Parse info and create a jobinfo.
        int i = (index >= MAX_ITEMS_PER_REQ)?(index%MAX_ITEMS_PER_REQ):(index);
        info = new JobInfo(parseForInfo(i),this);
        mData.insert(index,info);
    }
    return info;
}

QVariantMap JobModelPrivate::parseForInfo(int index) {
#if defined(TEST_WITH_DUMMY_DATA)
    // Return dummy data
    QVariantMap info;
    info.insert(NJ_PROP_KEY_ISVALID,true);
    info.insert(NJ_PROP_KEY_TITLE,QString("Qt Developer ")+QString().setNum((mDataOffset-1)*MAX_ITEMS_PER_REQ + index));
    info.insert(NJ_PROP_KEY_EMPNAME,QString("Dreamcode"));
    info.insert(NJ_PROP_KEY_LOCATION,QString("Hyderabad"));
    info.insert(NJ_PROP_KEY_DATE,QDate::currentDate().toString());
    QString testDesc = "Strong Experience in building multithreaded QT applicationsStrong knowledge in multithreaded programming.</br>Strong object oriented programming skills.Strong Experience in debugging and analysis skills to identify memory leaks and dead locks.Expertise in VS2008/10 IDE.";
    info.insert(NJ_PROP_KEY_DESCRIPTION,testDesc.simplified());
    return info;
#else
    qDebug()<<Q_FUNC_INFO<<index;
    RSSParser* parser = currentParser();
    QVariantMap info;
    info.insert(NJ_PROP_KEY_ISVALID,false);
    if(parser && index < parser->count()) {
        QString t = parser->itemElement(index,RSSParser::title);
        //Title format: CAE, II - Synopsys -  Hyderabad, Andhra Pradesh
        QStringList tmp = t.split(" - ");
        QString title;
        QString emp;
        QString loc;
        if(tmp.size() >= 2) {
            title = tmp.first();
            emp = tmp.at(tmp.size() - 2); // 2nd from last
            loc = tmp.last();
        } else {
            title = emp = loc = tmp.first();
        }

        info.insert(NJ_PROP_KEY_TITLE,title.trimmed());
        info.insert(NJ_PROP_KEY_EMPNAME,emp.trimmed());
        info.insert(NJ_PROP_KEY_LOCATION,loc.trimmed());
        QString d= parser->decodeHtml(parser->itemElement(index,RSSParser::description));
        info.insert(NJ_PROP_KEY_DESCRIPTION,d.simplified()); // Having some issue with this formatted text, hence simplifying.
        info.insert(NJ_PROP_KEY_URL,parser->itemElement(index,RSSParser::link));
        info.insert(NJ_PROP_KEY_DATE,parser->itemElement(index,RSSParser::pubDate).trimmed());
        info.insert(NJ_PROP_KEY_SOURCE,parser->itemElement(index,"source").trimmed());
        info.insert(NJ_PROP_KEY_ISVALID,true);
    } else {
        qDebug()<<Q_FUNC_INFO<<"Ignoring req for "<<index;
    }
return info;
#endif
}

void JobModelPrivate::resetModel() {
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

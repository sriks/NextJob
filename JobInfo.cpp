#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QDebug>
#include <QTimer>
#include <QPointer>
#include "JobInfo.h"
#include "Constants.h"

// start and end keys to parse for description.
const QString START_KEY("<span class=\"summary\">");
const QString END_KEY("</span>");
const int WAIT_PERIOD(6000);

struct JobInfoPrivate {
    bool isFav;
    bool ignoreTimer;
    QVariantMap info;
    QNetworkAccessManager* netMgr;
    QPointer<QNetworkReply> reply;

    JobInfoPrivate() {
        netMgr = NULL;
        ignoreTimer = false;
    }

    ~JobInfoPrivate() {
        delete netMgr;
    }
};

JobInfo::JobInfo(const QVariantMap data, QObject *parent)
        :QObject(parent) {
    d = new JobInfoPrivate;
    d->info = data;
    d->isFav = false;
}

JobInfo::~JobInfo() {
    delete d;
}

QVariantMap JobInfo::key() const {
    return d->info;
}

QString JobInfo::title() const {
    return d->info.value(NJ_PROP_KEY_TITLE).toString();
}

QString JobInfo::description() const {
    return d->info.value(NJ_PROP_KEY_DESCRIPTION).toString();
}

QString JobInfo::employer() const {
    return d->info.value(NJ_PROP_KEY_EMPNAME).toString();
}

QString JobInfo::location() const {
    return d->info.value(NJ_PROP_KEY_LOCATION).toString();
}

QUrl JobInfo::url() const {
    return d->info.value(NJ_PROP_KEY_URL).toUrl();
}

QString JobInfo::date() const {
    return d->info.value(NJ_PROP_KEY_DATE).toString();
}

bool JobInfo::isFavorite() const {
    return d->isFav;
}

bool JobInfo::setFavorite(bool fav) {
    d->isFav = fav;
    emit favoriteChanged();
}

void JobInfo::handleDetailedDescription(QNetworkReply *reply) {
    d->ignoreTimer = true;
    QString resp(reply->readAll());
    int startIndex = resp.indexOf(START_KEY,0,Qt::CaseInsensitive);
    startIndex += START_KEY.size();
    int endIndex = resp.indexOf(END_KEY,startIndex,Qt::CaseInsensitive);
    QString des = resp.mid(startIndex,endIndex-startIndex);
    if(!des.isEmpty()) {
        d->info.insert(NJ_PROP_KEY_DESCRIPTION,des);
        d->info.insert(NJ_PROP_KEY_DETAILED_DESCRIPTION_FETCHED,QVariant(true));
    }
    reply->deleteLater();
    emit descriptionChanged();
}

void JobInfo::abortDetailedDescription() {
    if(d->ignoreTimer)
        return;
    if(d->reply) {
        d->reply->abort();
        d->reply->deleteLater();
    }
    emit descriptionChanged();
}

/*!
  Fetches detailed description.
  Description from feed is very basic and not descriptive. Hence we do a GET of the url and
  parse for description.
  Once fetched, signal descriptionChanged is emitted.
  Simply ignores if this description is already parsed within a session and for favorites.
  For UX, a timer is started after which the attempt to GET is aborted.
  **/
void JobInfo::fetchDetailedDescription() {
    if(d->info.value(NJ_PROP_KEY_DETAILED_DESCRIPTION_FETCHED).toBool()) {
        emit descriptionChanged();
        return;
    }

    if(!d->netMgr) {
        d->netMgr = new QNetworkAccessManager(this);
        connect(d->netMgr,SIGNAL(finished(QNetworkReply*)),
                this,SLOT(handleDetailedDescription(QNetworkReply*)),
                Qt::UniqueConnection);
    }
    d->ignoreTimer = false;
    d->reply = d->netMgr->get(QNetworkRequest(d->info.value(NJ_PROP_KEY_URL).toUrl()));
    QTimer::singleShot(WAIT_PERIOD,this,SLOT(abortDetailedDescription()));
}

// eof

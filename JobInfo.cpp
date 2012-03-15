#include "JobInfo.h"
#include "Constants.h"

struct JobInfoPrivate {
    QVariantMap info;
    bool isFav;
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

// eof

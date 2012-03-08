#include "JobModel.h"
#include "JobModelPrivate.h"

JobModel::JobModel(QObject *parent) :
    QAbstractListModel(parent) {
    d = new JobModelPrivate(this);
    QHash<int, QByteArray> roles;
    roles[JobModel::JobInfo] = "jobinfo"; // Role name to get jobinfo
    setRoleNames(roles);
}

JobModel::~JobModel() {
    delete d;
}

int JobModel::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent)
    d->count();
}

QVariant JobModel::data(const QModelIndex &index, int role) const {
    return QVariant::fromValue((QObject*)d->at(index.row()));
}

void JobModel::addKey(QVariantMap key) {
    d->add(key);
}

void JobModel::next() {
    d->fetchMoreData();
}

int JobModel::count() const {
    return d->count();
}


// eof

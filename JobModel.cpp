#include "JobModel.h"
#include "JobModelPrivate.h"

JobModel::JobModel(JobModelType type,QObject *parent) :
    QAbstractListModel(parent) {
    d = new JobModelPrivate(this,type);
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

QVariantMap JobModel::key() const {
    return d->key();
}

QUrl JobModel::baseUrl() const {
    return d->mBaseUrl;
}

void JobModel::next() {
    d->fetchMoreData();
}

/*!
  Resets the model, the same data is again appended to list.
  **/
void JobModel::reset() {
    d->resetModel();
}

int JobModel::count() const {
    return d->count();
}


// eof

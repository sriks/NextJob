#include <QDebug>
#include "AlertModel.h"
#include "JobAlert.h"
#include "JobManager.h"

struct AlertModelPrivate {
    AlertModel::AlertModelType type;
    QList< QVariantMap > alertKeys;
    JobManager* mgr() {
        return JobManager::instance();
    }
};

AlertModel::AlertModel(AlertModelType type,QObject *parent) :
            QAbstractListModel(parent) {
    d = new AlertModelPrivate;
    d->type = type;
    QHash<int, QByteArray> roles;
    roles[AlertModel::AlertInfo] = "alertinfo"; // Role name to get jobinfo
    setRoleNames(roles);
}

AlertModel::~AlertModel() {
    delete d;
}

AlertModel::AlertModelType AlertModel::type() const {
    return d->type;
}

int AlertModel::rowCount(const QModelIndex &parent) const {
    return d->alertKeys.size();
}

QVariant AlertModel::data(const QModelIndex &index, int role) const {
    Q_UNUSED(role)
    QObject* a = d->mgr()->alert(d->alertKeys.at(index.row()));
    return qVariantFromValue(a);
}

void AlertModel::loadData() {
    QListIterator<JobAlert*> iter(d->mgr()->alerts());
    while(iter.hasNext())
        alertAdded(iter.next()->key());
}

void AlertModel::alertAdded(QVariantMap source) {
    bool added = false;
    int indexToAdd = -1;
    if(d->alertKeys.contains(source))
        return;
    JobAlert* a = d->mgr()->alert(source);
    if(type() == AlertModel::AlertsWithNewJobs) {
        if(a->newJobsCount()) {
            indexToAdd = 0;
            added = true;
        }
    } else if(type() == AlertModel::AllAlerts) {
        if(a->newJobsCount())
            indexToAdd = 0; // add at beginning
        else
            indexToAdd = d->alertKeys.size();
        added = true;
    }

    if(added) {
        d->alertKeys.insert(indexToAdd,source);
        this->beginInsertRows(QModelIndex(),indexToAdd,indexToAdd);
        this->endInsertRows();
    }
}

void AlertModel::alertRemoved(QVariantMap source) {
    int i = d->alertKeys.indexOf(source);
    if(i >= 0) {
        d->alertKeys.removeAt(i);
        this->beginRemoveRows(QModelIndex(),i,i);
        this->endRemoveRows();
    }
}

void AlertModel::alertUpdated(int newItemsCount, QVariantMap source) {
    alertRemoved(source);
    alertAdded(source);
}

int AlertModel::count() const {
    return d->alertKeys.size();
}

// eof

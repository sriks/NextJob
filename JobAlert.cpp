#include "Constants.h"
#include "JobAlert.h"
#include "JobModel.h"

struct JobAlertPrivate {
    QVariantMap key;
    int newItemCount;
    JobModel* model;

    ~JobAlertPrivate() {
        model->deleteLater();
    }
};

JobAlert::JobAlert(QVariantMap key,QObject *parent) :
    QObject(parent) {
    d = new JobAlertPrivate;
    d->key = key;
    d->model = NULL;
    d->newItemCount = 0;
}

JobAlert::~JobAlert() {
    delete d;
}

QVariantMap JobAlert::key() const {
    return d->key;
}

JobModel *JobAlert::model() const {
    return d->model;
}

int JobAlert::newItemsCount() const {
    return d->newItemCount;
}

void JobAlert::startUpdate() {
    if(d->model)
        delete d->model;
    d->model = new JobModel;
    d->model->addKey(d->key);
    connect(d->model,SIGNAL(updateAvailable(int)),
            this,SLOT(onUpdateAvailable(int)),
            Qt::UniqueConnection);
}

void JobAlert::onUpdateAvailable(int newItemsCount) {
    d->newItemCount = newItemsCount;
    emit updateAvailable(d->key,newItemsCount);
}

// eof

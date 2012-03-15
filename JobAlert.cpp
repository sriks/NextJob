#include <QDebug>
#include "Constants.h"
#include "JobAlert.h"
#include "JobModel.h"

struct JobAlertPrivate {
    QVariantMap key;
    int newItemCount;
    JobModel* model;
    bool visited;

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
    d->visited = false;
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

QString JobAlert::displayName() const {
    QString name("%1 - %2 - %3");
    return name.arg(key().value(NJ_SKILL).toString(),
              key().value(NJ_LOCATION).toString(),
              key().value(NJ_COUNTRY).toString()).simplified();
}

QString JobAlert::displayNameWithJobCount() const {
    QString name =("%1 (%2) new job(s)");
    return name.arg(displayName(),QString().setNum(d->newItemCount));
}

QString JobAlert::skill() const {
    return key().value(NJ_SKILL).toString();
}

QString JobAlert::location() const {
    return key().value(NJ_LOCATION).toString();
}

QString JobAlert::country() const {
    return key().value(NJ_COUNTRY).toString();
}

int JobAlert::newJobsCount() const {
    return d->newItemCount;
}

void JobAlert::update() {
    if(!d->model)
        d->model = new JobModel(JobModel::Alert,this);
    connect(d->model,SIGNAL(updateAvailable(int,QVariantMap)),
            this,SLOT(onUpdateAvailable(int,QVariantMap)),
            Qt::UniqueConnection);
    connect(this,SIGNAL(updateAvailable(int,QVariantMap)),
            this,SIGNAL(newJobsCountChanged()),
            Qt::UniqueConnection);
    d->model->addKey(d->key);
}

void JobAlert::onUpdateAvailable(int newItemsCount,QVariantMap key) {
    d->newItemCount = newItemsCount;
    if(newItemsCount)
        emit updateAvailable(newItemsCount,key);
}

void JobAlert::setVisited(bool val) {
    d->visited = val;
}

bool JobAlert::isVisited() const {
    return d->visited;
}

// eof

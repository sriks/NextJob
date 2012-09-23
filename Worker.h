#ifndef WORKER_H
#define WORKER_H

#include <QThread>
#include <QVariantMap>

class JobManagerPrivate;
class WorkerPrivate;
class Worker : public QThread
{
    Q_OBJECT
public:
    enum TaskType {
        RestoreState
    };

    explicit Worker(JobManagerPrivate* master,QObject *parent = 0);
    ~Worker();
    void setTask(TaskType task);
    TaskType task() const;
    QList< QVariantMap > favs() const;
    QSet<QString> favLookup() const;
    QList< QVariantMap > alertKeys() const;
    void clearData();
protected:
    // From QThread
    void run();
private:
    WorkerPrivate* d;
};

#endif // WORKER_H

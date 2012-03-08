#ifndef JOBINFO_H
#define JOBINFO_H

#include <QObject>
#include <QVariantMap>
#include <QUrl>

class JobInfoPrivate;
class JobInfo : public QObject
{
    Q_OBJECT
public:
    explicit JobInfo(const QVariantMap data,QObject *parent = 0);
    ~JobInfo();
signals:
    
public slots:
    bool isValid() const;
    QString title() const;
    QString employer() const;
    QString location() const;
    QString description() const;
    QUrl    url() const;
    bool    isFavorite() const;
    bool    setFavorite(bool fav);
private:
    JobInfoPrivate* d;
};

#endif // JOBINFO_H

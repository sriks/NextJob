#ifndef JOBINFO_H
#define JOBINFO_H

#include <QObject>
#include <QVariantMap>
#include <QUrl>

class JobInfoPrivate;
class JobInfo : public QObject
{
    Q_OBJECT
    //Q_PROPERTY(bool favorite READ isFavorite NOTIFY favoriteChanged)
public:
    explicit JobInfo(const QVariantMap data,QObject *parent = 0);
    ~JobInfo();
signals:
    void favoriteChanged();
public slots:
    QVariantMap key() const;
    //bool isValid() const;
    QString title() const;
    QString employer() const;
    QString location() const;
    QString description() const;
    QUrl    url() const;
    QString date() const;
//    bool    isFavorite() const;
//    bool    setFavorite(bool fav);
private:
    JobInfoPrivate* d;
};

#endif // JOBINFO_H

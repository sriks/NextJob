#ifndef JOBINFO_H
#define JOBINFO_H

#include <QObject>
#include <QVariantMap>
#include <QUrl>

class QNetworkReply;
class JobInfoPrivate;
class JobInfo : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool favorite READ isFavorite NOTIFY favoriteChanged)
    Q_PROPERTY(QString description READ description NOTIFY descriptionChanged)

public:
    explicit JobInfo(const QVariantMap data,QObject *parent = 0);
    ~JobInfo();
signals:
    void favoriteChanged();
    void descriptionChanged();
public slots:
    QVariantMap key() const;
    QString title() const;
    QString employer() const;
    QString location() const;
    QString description() const;
    void fetchDetailedDescription();
    QUrl    url() const;
    QString date() const;
    bool    isFavorite() const;
    bool    setFavorite(bool fav);

protected slots:
    void handleDetailedDescription(QNetworkReply*);
    void abortDetailedDescription();
private:
    JobInfoPrivate* d;
};

#endif // JOBINFO_H

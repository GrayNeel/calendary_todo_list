#ifndef CALENDAR_H
#define CALENDAR_H

#include <QObject>
#include <QAuthenticator>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QByteArray>
#include <QBuffer>
#include <QDomDocument>

#include "event.h"
#include "todo.h"

class Calendar : public QObject
{
    Q_OBJECT
public:
    explicit Calendar(QObject *parent = nullptr);
    ~Calendar();

    const QString &username() const;
    void setUsername(const QString &newUsername);
    const QString &password() const;
    void setPassword(const QString &newPassword);
    const QString &url() const;
    void setUrl(const QString &newUrl);

    const QString &displayName() const;

    const QList<Event *> &eventsList() const;

    const QString &colour() const;
    void setColour(const QString &newColour);

signals:
    void calendarRetrieveError(QString errorMessage);
    void calendarAdded();
    void removeCalendar(Calendar* cal);
    void showEventDialog(Calendar* cal);
    void eventAddFinished();
    void eventRetrieveError();

public slots:
    void handleAddEvent(QString summary, QString location, QString description, QDateTime startDateTime, QDateTime endDateTime);


private slots:
    void APIRequestSyncToken(void);
    void handleAuthentication(QNetworkReply *reply, QAuthenticator *authenticator) const;
    void checkResponseStatus();
    void handleRequestSyncTokenFinished();
    void APIRequestVCalendarObjects(void);
    void handleRequestVCalendarObjectsFinished();
    void handleRemoveCalendar();
    void handleAddNewEventPopUp();

    void handleGetFinished();



    void APIAddEvent(Event *event);
    void handleAddingVEventFinished();
private:
    QString _username;
    QString _password;
    QString _url;
    QNetworkReply *_reply;
    QNetworkAccessManager *_manager;
    QString _replyStatus;
    int _statusCode;

    QString _displayName;
    QString _cTag;
    QString _syncToken;

    QString _colour;

    QList<Event*> _eventsList;
    QList<Todo*> _todosList;
    void getForLastResource(QUrl resourceUrl);
    void parseResponse();
    void parseCalendarData(QString entity, QString uri, QString etag);
};

#endif // CALENDAR_H

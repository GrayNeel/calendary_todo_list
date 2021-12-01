#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QWidget>
#include <QAuthenticator>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QByteArray>
#include <QBuffer>
#include <QDomDocument>
#include "calendar.h"


class Client : public QObject
{
    Q_OBJECT
public:
    explicit Client(QObject *parent = nullptr);
    void addCalendar();

    const int getCalendarCount() const;

    void setCalendarList(const QList<Calendar *> &newCalendarList);

public slots:
    void handleAddCalendar(QString username, QString password, QString url);
    void handleAddCalendarError(QString errorMessage);
    void handleAddEvent(QString summary, QString location, QString description, QDateTime startDateTime, QDateTime endDateTime);
    void handleAddEventError(QString errorMessage);
    void handleRemoveCalendarFromList(Calendar* cal);

signals:
    void requestSyncToken(void);
    void dialogErrorMessage(QString errorMessage);
    void closeDialog(Calendar* cal);

    void eventDialogErrorMessage(QString errorMessage);
    void closeEventDialog(Event* event);

private:
    QList<Calendar*> _calendarList;


private slots:
    void handleAddCalendarFinished();

};

#endif // CLIENT_H

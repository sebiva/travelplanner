#ifndef TIMEHELPER_H
#define TIMEHELPER_H

#include <QObject>
#include <QDebug>
#include <QDateTime>
#include <QDate>
#include <QTime>
#include <QStringList>

class Timehelper : public QObject
{
    Q_OBJECT
public:
    explicit Timehelper(QObject *parent = 0);

    Q_INVOKABLE static QString convertdate(QString date);
    Q_INVOKABLE static QString converttime(QString hour, QString minute);
    Q_INVOKABLE static QString duration(QString depdate, QString deptime, QString arivdate, QString arivtime, bool delay = false);
    Q_INVOKABLE static QString delay(QString date, QString time, QString rtdate, QString rttime);
    Q_INVOKABLE static QString getcurrentdatestr();
    Q_INVOKABLE static QString getcurrenttimestr();
    Q_INVOKABLE static QDate getcurrentdate();
    Q_INVOKABLE static QTime getcurrenttime();
signals:

public slots:

};

#endif // TIMEHELPER_H

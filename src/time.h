#ifndef TIME_H
#define TIME_H

#include <QObject>

class Time : public QObject
{
    Q_OBJECT
public:
    explicit Time(QObject *parent = 0);

    Q_INVOKABLE static QString convertdate(QString date);
    Q_INVOKABLE static QString converttime(QString hour, QString minute);
signals:

public slots:

};

#endif // TIME_H

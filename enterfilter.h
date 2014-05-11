#ifndef ENTERFILTER_H
#define ENTERFILTER_H

#include <QObject>
#include <QEvent>
#include <QKeyEvent>

#include <QDebug>


class EnterFilter : public QObject
{
    Q_OBJECT
public:
    EnterFilter(QObject *parent = 0);
protected:
    bool eventFilter(QObject *obj, QEvent *event);
signals:
    void sendGovno();

public slots:

};

#endif // ENTERFILTER_H

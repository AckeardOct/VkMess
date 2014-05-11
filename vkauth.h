#ifndef VKAUTH_H
#define VKAUTH_H

#include <QWebView>
#include <QUrl>
#include <QUrlQuery>
#include <QString>
#include <QSettings>
#include "messages.h"

//Test
#include <QDebug>

class VkAuth : public QWebView
{
    Q_OBJECT
private:
    Messages  mess;
    QString token;
    QString mainUserId;
    QSettings * s_settings;
public:
    VkAuth(QWebView *parent = 0);
    ~VkAuth();
    void writeSettings();
    void readSettings();

signals:

public slots:
    void takeTokenSlot(QUrl cUrl);
};

#endif // VKAUTH_H

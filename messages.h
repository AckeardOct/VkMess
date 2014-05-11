#ifndef MESSAGES_H
#define MESSAGES_H

#include <QTextBrowser>
#include <QString>
#include <QTextEdit>
#include <QSplitter>
#include <QEvent>
#include <QKeyEvent>
#include <QDomDocument>
#include <QDomNode>
#include <QDomElement>
#include <QUrl>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QTimer>
#include <QFont>
#include <QPalette>
#include <QApplication>
#include <QStringList>
#include <QLabel>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QWidget>
#include <QSettings>
#include <QShortcut>


#include "enterfilter.h"


#include "bass.h"

#include <QDebug>

class Messages : public QSplitter
{
    Q_OBJECT
private:
    //тест ентера
    EnterFilter * filterEnter;

    QString accessToken;
    QString userId;
    QString dialogId;
    //QString dialogId = "135881409";
    QTextBrowser * history;
    QTextBrowser * htmlHistory;
    QTextEdit * sendText;
    QString textMessages;
    QString htmlMessages;
    QNetworkReply * reply;
    QNetworkAccessManager nam;
    QNetworkReply * replyOnline;
    QNetworkAccessManager namOnline;
    QTimer * timer;
    QTimer * timerOnline;
    QStringList messageIdsList;
    bool online;
    bool refreshOnline;
    bool top_bottom;
    float vol;
    QLineEdit * volumEdit;
    QPalette pal;
    QLabel * extLbl;
    QSettings * s_settings;
protected:
    virtual void keyPressEvent(QKeyEvent *ke);
public:
    Messages(QWidget *parent = 0);
    ~Messages();

    void setAccessToken(QString str);
    void setUserId(QString str);

    void setRead(QString message_id);
    void onlineLed();

    //Settings
    void writeSettings();
    void readSettings();

signals:


public slots:
    void messageHistorySlot();
    void downloadMessageHistorySlot();
    void sendMessageSlot();
    void onlineSlot();
    void downloadOnlineSlot();
    void setVolumeSlot();
};

#endif // MESSAGES_H

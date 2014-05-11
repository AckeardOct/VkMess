#include "messages.h"

void Messages::keyPressEvent(QKeyEvent *ke)
{
    switch(ke->key())
    {
    case Qt::Key_S:
        if(ke->modifiers() & Qt::ControlModifier)
            sendMessageSlot();
        break;

    case Qt::Key_F1:
        sendMessageSlot();
        break;

    case Qt::Key_F2:
        if(sendText->isActiveWindow())
        {
            volumEdit->setFocus();
        }
        break;
    case Qt::Key_F3:
        this->showMinimized();
        break;
    case Qt::Key_F4:
        if(top_bottom == false)
        {
            this->setWindowFlags(Qt::WindowSystemMenuHint | Qt::WindowCloseButtonHint | Qt::WindowStaysOnTopHint);
            top_bottom = true;
        }
        else
        {
            this->setWindowFlags(Qt::WindowSystemMenuHint | Qt::WindowCloseButtonHint | Qt::WindowStaysOnBottomHint);
            top_bottom = false;
        }
        this->show();
        this->activateWindow();
        break;
    default:
        QWidget::keyPressEvent(ke);

    }

}

Messages::Messages(QWidget *parent) :
    QSplitter(parent),
    filterEnter(new EnterFilter())
{    
    //INIT
    dialogId = "17757339"; //ATG
    //dialogId = "135881409";//Anko
    history = new QTextBrowser(this);
    htmlHistory = new QTextBrowser(this);
    sendText = new QTextEdit(this);
    timer = new QTimer(this);
    timerOnline = new QTimer(this);
    online=true;
    refreshOnline = true;
    top_bottom = false;
    volumEdit = new QLineEdit(this);
    extLbl = new QLabel;
    s_settings = new QSettings("set.ini",QSettings::IniFormat,this);


    this->sendText->installEventFilter(filterEnter);
    vol = 0.5;
    this->setWindowFlags(Qt::WindowSystemMenuHint | Qt::WindowCloseButtonHint );
    this->setWindowTitle("G < 3");
    this->setHandleWidth(0);
    this->resize(250,450);
    this->setFrameStyle(0);
    sendText->setAcceptRichText(false);

    history->hide();
    sendText->setFixedHeight(80);
    QFont  sendFont = sendText->font();
    sendFont.setPointSize(12);
    sendText->setFont(sendFont);
    sendText->setFrameStyle(0);
    sendText->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    htmlHistory->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    extLbl->setAutoFillBackground(true);
    extLbl->setFrameStyle(0);

    volumEdit->setFixedSize(17,17);
    volumEdit->setFrame(0);

    pal.setColor(QPalette::Base,QColor("#404040"));
    pal.setColor(extLbl->backgroundRole(),QColor("#404040"));
    pal.setColor(QPalette::Text, QColor("#A0A0A0"));
    extLbl->setPalette(pal);
    volumEdit->setPalette(pal);
    sendText->setPalette(pal);

    QPalette pal2;
    pal2.setColor(QPalette::Base, Qt::black);
    htmlHistory->setPalette(pal2);

    extLbl->setPalette(pal);
    volumEdit->setPalette(pal);



    emit messageHistorySlot();



    timer->start(500);
    timerOnline->start(5000);
    //Layout
    QSplitter * extPan = new QSplitter(this);
    extPan->setOrientation(Qt::Vertical);
    extPan->setHandleWidth(0);    
    extPan->addWidget(volumEdit);
    extPan->addWidget(extLbl);
    QSplitter * spl = new QSplitter(this);
    spl->setOrientation(Qt::Horizontal);
    spl->addWidget(sendText);
    spl->addWidget(extPan);
    spl->setHandleWidth(0);
    this->setOrientation(Qt::Vertical);    
    this->addWidget(spl);
    this->addWidget(htmlHistory);

    readSettings();

    //проба хоткея
    QShortcut *sh = new QShortcut(QKeySequence("Enter"),sendText);
        sh->connect(sh,SIGNAL(activated()),this,SLOT(sendMessageSlot()));
                sh->setContext( Qt::ApplicationShortcut );
                setShortcutEnabled(sh->id());


    connect(timer,SIGNAL(timeout()),
            this,SLOT(messageHistorySlot()));
    connect(timerOnline,SIGNAL(timeout()),
            this,SLOT(onlineSlot()));
    connect(volumEdit,SIGNAL(returnPressed()),
            this,SLOT(setVolumeSlot()));
    connect(filterEnter,SIGNAL(sendGovno()),SLOT(sendMessageSlot()));

}

Messages::~Messages()
{
    qDebug() << "Messages destruct";
    writeSettings();
}

void Messages::setAccessToken(QString str)
{
    accessToken = str;
}

void Messages::setUserId(QString str)
{
    userId = str;
}

void Messages::setRead(QString message_id)
{
    QString readUrl;
    readUrl = "https://api.vk.com/method/messages.markAsRead?";
    readUrl += "message_ids=" + message_id + "&";
    readUrl += "user_id=" + dialogId + "&";
    readUrl += "start_message_id=174964&";
    readUrl += "v=5.2&";
    readUrl += "access_token=" + accessToken;
    QNetworkRequest req(readUrl);
    reply = nam.get(req);
}

void Messages::onlineLed()
{
    if(refreshOnline == true)
    {
        //qDebug() << "refresh Led";
        if(online == true)
        {
            pal.setColor(QPalette::Base,QColor("#404040"));
            pal.setColor(extLbl->backgroundRole(),QColor("#404040"));
            volumEdit->setPalette(pal);
            sendText->setPalette(pal);
            extLbl->setPalette(pal);
        }
        else
        {            
            pal.setColor(QPalette::Base,Qt::black);
            pal.setColor(extLbl->backgroundRole(),Qt::black);
            volumEdit->setPalette(pal);
            sendText->setPalette(pal);
            extLbl->setPalette(pal);
        }
    }
}

void Messages::writeSettings()
{
    s_settings->beginGroup("/Settings/messages");

    s_settings->setValue("/geometry",geometry());

    s_settings->endGroup();
}

void Messages::readSettings()
{
    s_settings->beginGroup("/Settings/messages");

    setGeometry(s_settings->value("/geometry",QRect(200,200,300,300)).toRect());

    s_settings->endGroup();
}

void Messages::messageHistorySlot()
{
    QString strUrl;
        strUrl = "https://api.vk.com/method/messages.getHistory.xml?";
        strUrl += "count=10&";
        strUrl += "user_id=" + dialogId + "&";
        strUrl += "rev=0&";
        strUrl += "v=5.2&";
        strUrl += "access_token=" + accessToken;
        QNetworkRequest req(strUrl);
        reply = nam.get(req);
        connect(reply,SIGNAL(finished()),
                this,SLOT(downloadMessageHistorySlot()));

}

void Messages::downloadMessageHistorySlot()
{
    textMessages = "";
    htmlMessages = "";
    QStringList lst;
        QDomDocument text;
        //qDebug() << reply->readAll();
        text.setContent(reply->readAll());

        //вторая попытка
        if(text.hasChildNodes())
        {            
            QDomElement domElement = text.documentElement();
                QDomNode response = domElement.firstChild();// response
                //qDebug() << domElement.tagName();
                while(!response.isNull())
                {
                    QDomElement resEl = response.toElement();
                    if(resEl.isElement())
                    {
                        if(resEl.tagName() == "items")
                        {
                            QDomNode items = resEl.firstChild();
                            while(!items.isNull())
                            {
                                if(items.isElement())
                                {
                                    QDomNode messages = items.firstChild();
                                    QString fromId;
                                    QString body;
                                    QString htmlFromId;
                                    QString htmlBody;
                                    QString user_id;
                                    QString message_id = "";
                                    bool readState;
                                    while(!messages.isNull())
                                    {
                                        if(messages.isElement())
                                        {
                                            QDomElement messagesEl = messages.toElement();
                                            if(messagesEl.isElement())
                                            {

                                                if(messagesEl.tagName() == "from_id")
                                                {

                                                    lst << (messagesEl.text());                                                    
                                                    if(messagesEl.text() == userId)
                                                    {

                                                        fromId = "Alexander The Great\n";
                                                        htmlFromId = "<h3 align=right> <font color=#8D6BFF>Alexander The Great<br/></font>";
                                                        //fromId = "Anko no Baka\n";
                                                        //htmlFromId = "<h3 align=left> <font color=#8D6BFF>Anko no Baka<br/></font>";
                                                    }
                                                    else
                                                    {

                                                        fromId = "Anko no Baka\n";
                                                        htmlFromId = "<h3 align=left> <font color=#A0A0A0>Anko no Baka<br/></font>";
                                                        //fromId = "Alexander The Great\n";
                                                        //htmlFromId = "<h3 align=right> <font color=#A0A0A0>Alexander The Great<br/></font>";

                                                    }
                                                }

                                                if(messagesEl.tagName() == "body")
                                                {
                                                    body = messagesEl.text() + "\n";
                                                    htmlBody = messagesEl.text();
                                                }

                                                if(messagesEl.tagName() == "id")
                                                {
                                                    message_id = messagesEl.text();
                                                }

                                                if(messagesEl.tagName() == "user_id")
                                                {
                                                    user_id = messagesEl.text();

                                                }
                                                if(messagesEl.tagName() == "read_state")
                                                {
                                                    if(messagesEl.text() == "0")
                                                    {
                                                        readState = false;
                                                    }
                                                    else
                                                        readState = true;
                                                }

                                            }

                                        }

                                        messages = messages.nextSibling();
                                    }

                                    textMessages += fromId + body;

                                    if(lst.back() != userId && readState == false)
                                    {
                                        setRead(message_id);
                                        //qDebug() << "Read";
                                        readState = true;
                                    }

                                    if(lst.back() == userId)
                                    {
                                        htmlMessages += "<font color=#404040>" + htmlFromId + htmlBody + "<br/></font></h3>";
                                    }
                                    else
                                    {
                                        htmlMessages += "<font color=#572BB5>" + htmlFromId + htmlBody + "<br/></font></h3>";
                                    }

                                }
                                    items = items.nextSibling();
                            }
                        }
                    }
                    else
                        return;
                response = response.nextSibling();
            }

                if(history->toPlainText() != textMessages)
                {

                    //qDebug() << "Refresh";
                    history->setText(textMessages);
                    htmlHistory->setHtml(htmlMessages);

                    if(lst.front() != userId && !(this->isActiveWindow()))
                    {
                        //QApplication::beep();

                        QString fn("sms.wav");

                        BASS_Init(-1,44100,BASS_DEVICE_DEFAULT,0,NULL);


                        HSTREAM hstr = BASS_StreamCreateFile(false,fn.toLatin1(),0,0,0);
                        if(hstr)
                        {
                            BASS_ChannelSetAttribute(hstr, BASS_ATTRIB_VOL, vol);
                            BASS_ChannelPlay(hstr,false);
                        }

                    }
                    //qDebug() << "refresh";
                }
        }
        //else
            //qDebug() << "Error";

}

void Messages::sendMessageSlot()
{
    QString strUrl;
        strUrl = "https://api.vk.com/method/messages.send.xml?";;
        strUrl += "user_ids=" + dialogId + "&";
        strUrl += "message=" + QString(sendText->toPlainText()) + "&";
        strUrl += "type=1&";
        strUrl += "v=5.2&";
        strUrl += "access_token=" + accessToken;
        QNetworkRequest req(strUrl);
        reply = nam.get(req);
        sendText->clear();
}

void Messages::onlineSlot()
{
    QString strUrl;
        strUrl = "https://api.vk.com/method/users.get.xml?";
        strUrl += "user_ids=" + dialogId + "&";
        strUrl += "fields=online&";
        strUrl += "v=5.2&";
        strUrl += "access_token=" + accessToken;
        QUrl statusUrl(strUrl);
        QNetworkRequest reqOnline(statusUrl);
        replyOnline = namOnline.get(reqOnline);
        connect(replyOnline,SIGNAL(finished()),
                this,SLOT(downloadOnlineSlot()));
}

void Messages::downloadOnlineSlot()
{
    bool temp = online;
    QDomDocument text;
    text.setContent(replyOnline->readAll());

    QDomElement domElement = text.documentElement();//response
    if(domElement.isElement())
    {
        QDomNode responseNode = domElement.firstChild();
        if(responseNode.isElement())
        {
            QDomNode node = responseNode.firstChild();
            QDomElement el1 = node.nextSiblingElement().nextSiblingElement().nextSiblingElement();            
            if(el1.text() == "1")
            {
                online = true;
            }
            else
            {
                online = false;
            }
        }
    }

    if(temp != online)
        refreshOnline = true;
    else
        refreshOnline = false;
    onlineLed();
}

void Messages::setVolumeSlot()
{
    QString str = volumEdit->text();
    float x = str.toFloat();   
    volumEdit->clear();
    if(x <= 10 && x >= 0)
    {
        vol = x/10;

        QString fn("sms.wav");

        BASS_Init(-1,44100,BASS_DEVICE_DEFAULT,0,NULL);


        HSTREAM hstr = BASS_StreamCreateFile(false,fn.toLatin1(),0,0,0);
        if(hstr)
        {
            BASS_ChannelSetAttribute(hstr, BASS_ATTRIB_VOL, vol);
            BASS_ChannelPlay(hstr,false);
        }
        //sendText->setFocus();
    }
}

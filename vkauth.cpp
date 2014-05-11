#include "vkauth.h"


VkAuth::VkAuth(QWebView *parent) :
    QWebView(parent)
{
    //INIT
    token = "";
    mainUserId = "";
    s_settings = new QSettings("set.ini",QSettings::IniFormat,this);


    this->setWindowFlags(Qt::WindowSystemMenuHint | Qt::WindowCloseButtonHint);
    this->resize(220,345);
    this->setWindowTitle("Авторизация");
    this->setWindowIcon(QPixmap("://source/logo.gif"));
    mess.setWindowIcon(QPixmap("://source/logo.gif"));


    //Auth
    QString authUrlString("https://oauth.vk.com/authorize?");
    authUrlString += "client_id=3868990&";
    authUrlString += "scope=messages&";
    authUrlString += "redirect_uri=http://vk.com&";
    authUrlString += "response_type=token&";
    authUrlString += "display=mobile&";
    authUrlString += "v=5.2";
    this->load(QUrl(authUrlString));

    readSettings();

    //connect
    connect(this,SIGNAL(urlChanged(QUrl)),
            this,SLOT(takeTokenSlot(QUrl)));
}

VkAuth::~VkAuth()
{
    //qDebug() << "Авторизация удалилась!";
    qDebug() << width() << height() << x() << y();
    writeSettings();
}

void VkAuth::writeSettings()
{
    s_settings->beginGroup("/Settings/vkauth");

    s_settings->setValue("/geometry",geometry());

    s_settings->endGroup();
}

void VkAuth::readSettings()
{
    s_settings->beginGroup("/Settings/vkauth");

    setGeometry(s_settings->value("/geometry",QRect(200,200,300,300)).toRect());

    s_settings->endGroup();
}


void VkAuth::takeTokenSlot(QUrl cUrl)
{
    if(token != 0)
    {
        this->close();
        return;
    }

    cUrl = cUrl.toString().replace("#","?");
    QUrlQuery qUrl(cUrl);
    token = qUrl.queryItemValue("access_token");
    mainUserId = qUrl.queryItemValue("user_id");

    if(token != 0)
    {
        mess.setAccessToken(token);
        mess.setUserId(mainUserId);
        mess.show();
        this->close();

    }
}

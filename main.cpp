#include <QApplication>
#include "vkauth.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setOrganizationName("Alexander The Great");
    a.setApplicationName("G Less Three Messenger");

    VkAuth vkAuth;

    vkAuth.show();



    return a.exec();
}

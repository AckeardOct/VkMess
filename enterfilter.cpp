#include "enterfilter.h"

EnterFilter::EnterFilter(QObject *parent) :
    QObject(parent)
{
}

bool EnterFilter::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::KeyPress)
        {

            QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
            if(keyEvent->key() == Qt::Key_Return /*& keyEvent->modifiers() == Qt::AltModifier*/)
            {
                emit sendGovno();
                return true;
            }
        }
        return QObject::eventFilter(obj, event);
}

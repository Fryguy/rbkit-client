#ifndef SUBSCRIBER_H
#define SUBSCRIBER_H

#include <QObject>
#include <QVariantMap>
#include "objectdetail.h"

// forward declaration of nzmqt classes
namespace nzmqt
{
   class ZMQContext;
   class ZMQSocket;
}

class QTimer;

class Subscriber : public QObject
{
    Q_OBJECT

    nzmqt::ZMQContext* m_context;
    nzmqt::ZMQSocket* m_socket;

    // Mapping of object ids and object type
    QMap<QString, QString> m_objId2Type;
    //
    QMap<QString, int> m_event2Count;

    // we are interested in this count.
    // this is count of each object type and how many of them exists.
    QVariantMap m_type2Count;

    // add a timer to emit stats
    QTimer* m_timer;

public:
    explicit Subscriber(QObject *parent = 0);
    ~Subscriber();

signals:
    void messageReady(const QVariantMap& map);
    void connected();
    void disconnected();
    void errored(const QString &);

public slots:
    void startListening(const QString &);
    void onMessageReceived(const QList<QByteArray>&);
    void onTimerExpiry();
};

#endif // SUBSCRIBER_H

#ifndef SUBSCRIBER_H
#define SUBSCRIBER_H

#include <QObject>
#include <QVariantMap>
#include <iostream>
#include <algorithm>
#include <vector>
#include <map>
#include <string>
#include <msgpack.hpp>

// forward declaration of nzmqt classes
namespace nzmqt
{
   class ZMQContext;
   class ZMQSocket;
}

class QTimer;
typedef std::map<std::string, msgpack::object> MapStrMsgPackObj;
typedef void (*eventFunctions)(MapStrMsgPackObj &);

class Subscriber : public QObject
{
    Q_OBJECT

    nzmqt::ZMQContext* m_context;
    nzmqt::ZMQSocket* m_socket;

    QMap<QString, QString> m_objId2Type;
    QMap<QString, int> m_event2Count;
    std::map<std::string, eventFunctions> eventFunctionMap;

    // we are interested in this count.
    QVariantMap m_type2Count;

    // add a timer to emit stats
    QTimer* m_timer;

public:
    explicit Subscriber(QObject *parent = 0);
    void populateEventFunctions();
    void processGcStatEvent(MapStrMsgPackObj& parsedMap);
    void newObjectEvent(MapStrMsgPackObj& parsedMap);
    void objectDestroyEvent(MapStrMsgPackObj& parsedMap);
    void gcStartEvent(MapStrMsgPackObj& parsedMap);
    void gcEndEvent(MapStrMsgPackObj& parsedMap);
    void objSpaceDumpEvent(MapStrMsgPackObj& parsedMap);
    void displayParsedMap(MapStrMsgPackObj& parsedMap);
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

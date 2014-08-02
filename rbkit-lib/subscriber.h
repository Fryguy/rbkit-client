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


namespace RBKit
{
    class ZmqCommandSocket;
    class ZmqEventSocket;
}

class Subscriber : public QObject
{
    Q_OBJECT
    typedef void (Subscriber::*EventMemberFunction)(MapStrMsgPackObj &, std::string &);
    std::map<std::string, EventMemberFunction> eventFunctionMap;

    RBKit::ZmqCommandSocket* commandSocket;
    RBKit::ZmqEventSocket* eventSocket;

    QMap<QString, QString> m_objId2Type;
    QMap<QString, int> m_event2Count;


    // we are interested in this count.
    QVariantMap m_type2Count;

    // add a timer to emit stats
    QTimer* m_timer;

public:
    explicit Subscriber(QObject *parent = 0);
    void populateEventFunctions();
    void processGcStatEvent(MapStrMsgPackObj& parsedMap, std::string& eventName);
    void newObjectEvent(MapStrMsgPackObj& parsedMap, std::string& eventName);
    void objectDestroyEvent(MapStrMsgPackObj& parsedMap, std::string& eventName);
    void gcStartEvent(MapStrMsgPackObj& parsedMap, std::string& eventName);
    void gcEndEvent(MapStrMsgPackObj& parsedMap, std::string& eventName);
    void objSpaceDumpEvent(MapStrMsgPackObj& parsedMap, std::string& eventName);
    void displayParsedMap(MapStrMsgPackObj& parsedMap);
    ~Subscriber();

signals:
    void messageReady(const QVariantMap& map);
    void connected();
    void disconnected();
    void errored(const QString &);

public slots:
    void startListening(QString, QString);
    void stop();
    void onMessageReceived(const QList<QByteArray>&);
    void onTimerExpiry();
};

#endif // SUBSCRIBER_H

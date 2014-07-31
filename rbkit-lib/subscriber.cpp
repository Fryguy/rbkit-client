#include <QDebug>
#include <QThread>
#include <QTimer>

#include "nzmqt/nzmqt.hpp"

#include "subscriber.h"
#include "gcstat.h"

static const int rbkcZmqTotalIoThreads = 1;
static const int timerIntervalInMs = 1000;

Subscriber::Subscriber(QObject *parent) :
    QObject(parent)
{
    populateEventFunctions();
    // second argument to creating a context is number of io threads. right
    // now, we are using only 1 thread, so defaulting to 1 for now.
    m_context = new nzmqt::SocketNotifierZMQContext(this, rbkcZmqTotalIoThreads);
    m_socket = m_context->createSocket(nzmqt::ZMQSocket::TYP_SUB, this);
    m_socket->setOption(nzmqt::ZMQSocket::OPT_SUBSCRIBE, "", 0);

    connect(m_socket, SIGNAL(messageReceived(const QList<QByteArray>&)),
            this, SLOT(onMessageReceived(const QList<QByteArray>&)));

    // initialize the timer, mark it a periodic one, and connect to timeout.
    m_timer = new QTimer(this);
    m_timer->setInterval(timerIntervalInMs);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(onTimerExpiry()));
}

void Subscriber::populateEventFunctions() {
    eventFunctionMap["gc_stats"] = &Subscriber::processGcStatEvent;
    eventFunctionMap["obj_created"] = &Subscriber::newObjectEvent;
    eventFunctionMap["obj_destroyed"] = &Subscriber::objectDestroyEvent;
    eventFunctionMap["gc_start"] = &Subscriber::gcStartEvent;
    eventFunctionMap["gc_end_s"] = &Subscriber::gcEndEvent;
    eventFunctionMap["object_space_dump"] = &Subscriber::objSpaceDumpEvent;
}

void Subscriber::processGcStatEvent(MapStrMsgPackObj &parsedMap, std::string& eventName) {
    std::cout << "Got a gc stat event" << endl;
    GcStat *stat = new GcStat(eventName, parsedMap);
    displayParsedMap(parsedMap);
}

void Subscriber::newObjectEvent(MapStrMsgPackObj &parsedMap, std::string& eventName) {
    std::cout << "Got new object event" << std::endl;
}

void Subscriber::objectDestroyEvent(MapStrMsgPackObj &parsedMap, std::string& eventName) {
    std::cout << "Got object destroy event" << std::endl;

}

void Subscriber::gcEndEvent(MapStrMsgPackObj &parsedMap, std::string& eventName) {
    std::cout << "Got gc end event" << std::endl;

}

void Subscriber::gcStartEvent(MapStrMsgPackObj &parsedMap, std::string& eventName) {
    std::cout << "Got gc start event" << std::endl;

}

void Subscriber::objSpaceDumpEvent(MapStrMsgPackObj &parsedMap, std::string& eventName) {
    std::cout << "Got objectspace dump" << std::endl;
}


Subscriber::~Subscriber()
{
    m_socket->close();
    delete m_socket;
    delete m_context;
    emit disconnected();
}

void Subscriber::startListening(const QString& host)
{
    qDebug() << "Got " << host;
    QByteArray ba = host.toLocal8Bit();
    const char *hostString = ba.data();
    try
    {
        m_socket->connectTo(hostString);
    }
    catch(zmq::error_t err)
    {
        QString str = QString::fromUtf8(err.what());
        qDebug() << str ;
        emit  errored(str);
    }

    emit connected();

    qDebug() << m_context->isStopped();
    if (!m_context->isStopped()) {
        m_context->start();
        m_timer->start();
    }
    qDebug() << "started";

}


void Subscriber::onMessageReceived(const QList<QByteArray>& rawMessage)
{
    for (QList<QByteArray>::ConstIterator iter = rawMessage.begin();
         rawMessage.end() != iter; ++iter)
    {
        const QByteArray& message = *iter;
        msgpack::unpacked unpackedMessage;
        msgpack::unpack(&unpackedMessage, message.data(), message.size());
        msgpack::object obj = unpackedMessage.get();
        //FIXME: We will have to free the memory allocated here.
        MapStrMsgPackObj parsedMap = obj.as<MapStrMsgPackObj>();

        MapStrMsgPackObj::iterator msgpackIterator;
        msgpackIterator = parsedMap.find("event_type");
        if(msgpackIterator != parsedMap.end()) {
            std::string eventName = msgpackIterator->second.as<std::string>();
            std::map<std::string, EventMemberFunction>::iterator eventIterator = eventFunctionMap.find(eventName);
            if(eventIterator != eventFunctionMap.end()) {
                EventMemberFunction functionPtr = eventIterator->second;
                (this->*functionPtr)(parsedMap, eventName);
            }
        }
    }
}

void Subscriber::displayParsedMap(MapStrMsgPackObj &parsedMap) {
    MapStrMsgPackObj::iterator msgpackIterator;
    for(msgpackIterator = parsedMap.begin();
        msgpackIterator != parsedMap.end();
        msgpackIterator++) {
        std::cout << msgpackIterator->first << std::endl;
        std::cout << msgpackIterator->second << std::endl;
    }
}

void Subscriber::onTimerExpiry()
{
    qDebug() << m_type2Count;
    emit messageReady(m_type2Count);
}

#include "testobjectdump.h"
#include "rbevents.h"
#include "model/objectstore.h"
#include <QDebug>

extern QVariantMap parseMsgpackObjectMap(const msgpack::object_map& obj);
using namespace RBKit;

static QByteArray msgpackDataFromFile(const QString filename)
{
    QFile file(filename);
    file.open(QFile::ReadOnly);
    return file.readAll();
}

void TestObjectDump::initTestCase()
{
    // read object dump, and parse it
    objectDump = msgpackDataFromFile(":/tests/msgpack/hugedump");

    msgpack::unpacked unpackedMessage;
    msgpack::unpack(&unpackedMessage, objectDump.data(), objectDump.size());

    auto evt = parseEvent(objectDump);
    event.reset(dynamic_cast<EvtObjectDump *>(evt));
}

void TestObjectDump::testBenchmarkUnpackObjectDump()
{
    QBENCHMARK_ONCE {
        msgpack::unpacked unpackedMessage;
        msgpack::unpack(&unpackedMessage, objectDump.data(), objectDump.size());
    }
}

void TestObjectDump::testBenchmarkParseObjectDump()
{
    msgpack::unpacked unpackedMessage;
    msgpack::unpack(&unpackedMessage, objectDump.data(), objectDump.size());

    QBENCHMARK_ONCE {
        msgpack::object_map obj = unpackedMessage.get().via.map;
        QVariantMap map = parseMsgpackObjectMap(obj);
        qDebug() << 'total in map:' << event->objects.size();
    }
}

void TestObjectDump::testBenchmarkMakeEventFromObjectDump()
{
    msgpack::unpacked unpackedMessage;
    msgpack::unpack(&unpackedMessage, objectDump.data(), objectDump.size());

    msgpack::object_map obj = unpackedMessage.get().via.map;
    QVariantMap map = parseMsgpackObjectMap(obj);

    EventDataBase* event(NULL);
    QBENCHMARK_ONCE {
        event = makeEventFromQVariantMap(map);
    }
}


void TestObjectDump::testBenchmarkProcessObjectsWhenObjectSpaceIsEmpty()
{
    qDebug() << "total objects :" << event->objects.size();

    // Create an objectstore
    ObjectStore store;

    qDebug() << "populating object store for first time";
    QBENCHMARK {
        store.updateFromSnapshot(event->objects);
    }
}

void TestObjectDump::testBenchmarkProcessObjectsWhenObjectSpaceIsFull()
{
    qDebug() << "total objects :" << event->objects.size();

    // Create an objectstore
    ObjectStore store;

    store.updateFromSnapshot(event->objects);

    qDebug() << "populating object store again";
    QBENCHMARK {
        store.updateFromSnapshot(event->objects);
    }
}

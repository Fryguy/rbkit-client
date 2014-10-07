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
    }
}

void TestObjectDump::testBenchmarkMakeEventFromObjectDump()
{
    msgpack::unpacked unpackedMessage;
    msgpack::unpack(&unpackedMessage, objectDump.data(), objectDump.size());

    msgpack::object_map obj = unpackedMessage.get().via.map;
    QVariantMap map = parseMsgpackObjectMap(obj);

    QSharedPointer<EvtObjectDump> event;
    QBENCHMARK_ONCE {
        auto evt = makeEventFromQVariantMap(map);
        event.reset(dynamic_cast<EvtObjectDump *>(evt));
    }
}

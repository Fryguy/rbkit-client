#include "testobjectdump.h"
#include "rbevents.h"
#include "mpparser.h"
#include "model/objectstore.h"
#include <QDebug>

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
        msgpack::object obj = unpackedMessage.get();
        auto map = obj.as< QMap<QString, msgpack::object> >();
        qDebug() << map.size();
    }
}

void TestObjectDump::testBenchmarkMakeEventFromObjectDump()
{
    QSharedPointer<EvtObjectDump> event;
    QBENCHMARK_ONCE {
        auto evt = RBKit::parseEvent(objectDump);
        event.reset(dynamic_cast<EvtObjectDump *>(evt));
    }
}

void TestObjectDump::testBenchmarkMakeEvent1FromObjectDump()
{
    QSharedPointer<EvtObjectDump> event;
    QBENCHMARK_ONCE {
        auto evt = RBKit::parseEvent1(objectDump);
        event.reset(dynamic_cast<EvtObjectDump *>(evt));
    }
}

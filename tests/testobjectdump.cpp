#include "testobjectdump.h"
#include "rbevents.h"
#include "model/objectstore.h"
#include "sqlconnectionpool.h"
#include <QDebug>
#include "mpparser.h"

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
    // objectDump = msgpackDataFromFile(":/tests/msgpack/hugedump");

    // auto evt = parseEvent1(objectDump);
    // event.reset(dynamic_cast<EvtObjectDump *>(evt));
}

void TestObjectDump::testBenchmarkParseObjectDump()
{
    // EventDataBase* base = NULL;
    // QBENCHMARK {
    //     base = parseEvent1(objectDump);
    // }

    // QVERIFY(base);

    // EvtObjectDump* event = dynamic_cast<EvtObjectDump *>(base);
    // QVERIFY(event);

    // qDebug() << "total objects :" << event->objects.size();
}


void TestObjectDump::testBenchmarkProcessObjectsWhenObjectSpaceIsEmpty()
{
    // qDebug() << "total objects :" << event->objects.size();

    // Create an objectstore
    // ObjectStore store;

    // qDebug() << "populating object store for first time";
    // QBENCHMARK {
    //     store.updateFromSnapshot(event->objects);
    // }
}

void TestObjectDump::testBenchmarkProcessObjectsWhenObjectSpaceIsFull()
{
    // qDebug() << "total objects :" << event->objects.size();

    // Create an objectstore
    // ObjectStore store;

    // store.updateFromSnapshot(event->objects);

    // qDebug() << "populating object store again";
    // QBENCHMARK {
    //     store.updateFromSnapshot(event->objects);
    // }
}


void TestObjectDump::testBenchmarkPopulateListWithObjects()
{
    QByteArray message = msgpackDataFromFile(":/tests/msgpack/objcreated");

    msgpack::unpacked unpackedMessage;
    msgpack::unpack(&unpackedMessage, message.data(), message.size());

    msgpack::object obj = unpackedMessage.get().via.map.ptr[2].val;

    // took this much time: 22,546 msecs
    // QBENCHMARK_ONCE {
    //     QList<ObjectDetailPtr> list;
    //     for (int iter = 0; iter != 2000000; ++iter) {
    //         list.append(obj.as<ObjectDetailPtr>());
    //     }
    //     qDebug() << list.size();
    // }

    // took this much time: 16,180 msecs
    // QBENCHMARK_ONCE {
    //     QList<ObjectDetail> list;
    //     for (int iter = 0; iter != 2000000; ++iter) {
    //         ObjectDetail object;
    //         obj >> object;
    //         list.append(object);
    //     }
    //     qDebug() << list.size();
    // }

    // took this much time: 13,631 msecs
    // QBENCHMARK {
    //     for (int iter = 0; iter != 2000000; ++iter) {
    //         ObjectDetail object;
    //         obj >> object;
    //     }
    // }

    // directly saving to database, by parsing some stuff.
    auto instance = SqlConnectionPool::getInstance();
    instance->setupDatabase();
    instance->prepareTables();

    ObjectDetail object;
    obj >> object;

    QBENCHMARK_ONCE {
        if (!instance->query.exec(QString("begin transaction"))) {
            qDebug() << instance->query.lastError();
        }
        if (!instance->query.prepare(
                QString("insert into rbkit_objects_%0(id, class_name, size, reference_count, file) values (?, ?, ?, ?, ?)")
                .arg(0))) {
            qDebug() << instance->query.lastError();
            return;
        }

        for (int iter = 0; iter != 2000000; ++iter) {
            instance->query.addBindValue(object.objectId);
            instance->query.addBindValue(object.className);
            instance->query.addBindValue(object.size);
            instance->query.addBindValue(object.references.size());
            instance->query.addBindValue(object.getFileLine());

            // if (!instance->query.prepare(QString("insert into rbkit_object_references_%0(object_id, child_id) values (?, ?)").arg(1)))
            //     qDebug() << instance->query.lastError();

            // objectStore->insertReferences(instance->query);
        }

        if (!instance->query.exec(QString("commit transaction")))
            qDebug() << instance->query.lastError();

    }
}

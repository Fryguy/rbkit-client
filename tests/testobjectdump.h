#ifndef TEST_RBKIT_OBJECTDUMP_H
#define TEST_RBKIT_OBJECTDUMP_H

#include <QSharedPointer>
#include "AutoTest.h"

// forward declaration
namespace RBKit
{
    class EvtObjectDump;
}

class TestObjectDump : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();

    void testBenchmarkUnpackObjectDump();
    void testBenchmarkParseObjectDump();
    void testBenchmarkMakeEvent1FromObjectDump();
    void testBenchmarkMakeEventFromObjectDump();

private:
    QByteArray objectDump;
    QSharedPointer<RBKit::EvtObjectDump> event;
};

DECLARE_TEST(TestObjectDump)

#endif // TEST_RBKIT_OBJECTDUMP_H

#include "sqlconnectionpool.h"

RBKit::SqlConnectionPool RBKit::SqlConnectionPool::singleton = NULL;

RBKit::SqlConnectionPool::SqlConnectionPool()
    : snapShotVersion(0) {
}

RBKit::SqlConnectionPool* RBKit::SqlConnectionPool::getInstance() {
    if(!singleton) {
        singleton = new RBKit::SqlConnectionPool();
        return singleton;
    } else {
        return singleton;
    }
}

QSqlError RBKit::SqlConnectionPool::setupDatabase() {
    database = QSqlDatabase::addDatabase("QSQLITE");
    database.setDatabaseName("/tmp/rbkit.db");

    if (!database.open()) {
        return database.lastError();
    }

    query = QSqlQuery(database);
}


QVector<QString> RBKit::SqlConnectionPool::createTableSchemas() {
    QVector<QString> objectCreation;
    objectCreation.append(QString("create table rbkit_objects_%0(id integer primary key"
                                  ", class_name varchar, size integer "
                                  ", reference_count integer, file varchar"
                                  ", line integer)").arg(snapShotVersion));
    objectCreation.append(QString("create table rbkit_object_references_%0(id integer primary key asc"
                                  ", object_id integer"
                                  ", child_id integer)").arg(snapShotVersion));
    return objectCreation;
}

QSqlError RBKit::SqlConnectionPool::insertDummyData() {
    if (!query.prepare(QString("insert into rbkit_objects(id, class_name, size, reference_count, file, line) values (?, ?, ?, ?, ?, ?)")))
        return query.lastError();
    addObject(1000, "Hash", 10, 3, "foo.rb", 100);
    addObject(2000, "Array", 10, 3, "foo.rb", 100);
    addObject(3000, "Foo", 10, 3, "foo.rb", 100);
    addObject(4000, "String", 10, 3, "foo.rb", 100);
    addObject(4001, "String", 10, 3, "foo.rb", 100);
    addObject(4002, "String", 10, 3, "foo.rb", 100);
    addObject(4003, "String", 10, 3, "foo.rb", 100);
    addObject(4004, "String", 10, 3, "foo.rb", 100);

    if (!query.prepare(QString("insert into rbkit_object_references(object_id, child_id) values (?, ?)")))
        return query.lastError();

    addReference(1000, 2000);
    addReference(1000, 4000);
    addReference(2000, 4001);
    addReference(2000, 4002);
    addReference(2000, 4003);

    return QSqlError();
}

void RBKit::SqlConnectionPool::addReference(int parentId, int childId) {
    query.addBindValue(parentId);
    query.addBindValue(childId);
    query.exec();
}

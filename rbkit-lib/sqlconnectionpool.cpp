#include "sqlconnectionpool.h"

RBKit::SqlConnectionPool RBKit::SqlConnectionPool::singleton = NULL;

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
    QVector<QString> creationVector;
}

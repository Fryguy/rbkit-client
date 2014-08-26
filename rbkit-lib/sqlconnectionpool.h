#ifndef SQLCONNECTION_POOL_H
#define SQLCONNECTION_POOL_H

#include <QDebug>
#include <QSqlDatabase>
#include <QtSql>
#include <QSqlQuery>
#include <QSqlError>

namespace RBKit {
    class SqlConnectionPool {
    private:
        SqlConnectionPool();
        static SqlConnectionPool *singleton;
        QSqlDatabase database;
        QSqlQuery query;
        int snapShotVersion;
    public:
        QSqlError setupDatabase();
        static SqlConnectionPool* getInstance();
        QVector<QString> createTableSchemas();
    }
}

#endif // SQLCONNECTION_POOL_H

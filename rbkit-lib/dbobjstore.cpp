#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>
#include "dbobjstore.h"


RBKit::DbObjStore::DbObjStore(QObject *parent)
    : QObject(parent)
{
}

RBKit::DbObjStore::~DbObjStore()
{
}

void RBKit::DbObjStore::persistToDb(const RBKit::ObjectStore& store)
{
    QSqlDatabase database = QSqlDatabase::addDatabase("QSQLITE");
    QString path("/Users/yuva/rbkit.sqlite");
    database.setDatabaseName(path);

    if (!database.open()) {
        qDebug() << "unable to open db";
        return;
    }

    bool ret = false;
    QSqlQuery query1;
    ret = query1.exec("create table objects (id integer primary key)");
    qDebug() << ret;
    qDebug() << database.lastError();

    QSqlQuery query2;
    ret = query2.exec("create table refs (id integer primary key, obj_id integer, ref_id integer)");
    qDebug() << ret;
    qDebug() << database.lastError();


    quint64 refId = 1;

    QHash<quint64, RBKit::ObjectDetail*>::const_iterator iter =
        store.objectStore.constBegin();
    for (; iter != store.objectStore.constEnd(); ++iter) {
        QSqlQuery insertObject;
        insertObject.prepare("INSERT INTO objects (id) VALUES (?)");
        insertObject.addBindValue(iter.key());
        ret = insertObject.exec();
        // qDebug() << ret;
        // qDebug() << database.lastError();

        RBKit::ObjectDetail& object = *iter.value();
        QList<quint64>::const_iterator refIter = object.references.constBegin();
        for (; refIter != object.references.constEnd(); ++refIter) {
            QSqlQuery insertRef;
            insertRef.prepare("INSERT INTO refs (id, obj_id, ref_id) VALUES (?, ?, ?)");
            insertRef.addBindValue(refId);
            insertRef.addBindValue(iter.key());
            insertRef.addBindValue(*refIter);
            ret = insertRef.exec();
            // qDebug() << ret;
            // qDebug() << database.lastError();


            ++refId;
        }

    }
}

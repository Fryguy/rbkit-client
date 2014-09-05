#include "objectstore.h"

RBKit::ObjectStore::ObjectStore()
{
}

// Implement the copy constructor
RBKit::ObjectStore::ObjectStore(const ObjectStore &original)
{
    QHash<quint64, RBKit::ObjectDetail*>::const_iterator iter = original.objectStore.constBegin();
    while(iter != original.objectStore.constEnd()) {
        objectStore[iter.key()] = new ObjectDetail(*iter.value());
        ++iter;
    }
    objectTypeCount = QHash<QString, quint32>(original.objectTypeCount);
}

void RBKit::ObjectStore::insertObjectsInDB(QSqlQuery query)
{
    qDebug() << "*  Insert number of  objects : " << objectStore.size();
    QHash<quint64, RBKit::ObjectDetail*>::const_iterator iter = objectStore.constBegin();
    int batchSize = 0;
    QVariantList objectids;
    QVariantList classNames;
    QVariantList sizes;
    QVariantList refCounts;
    QVariantList locations;

    while(iter != objectStore.constEnd()) {
        ObjectDetail *objectDetail = iter.value();
        objectids << objectDetail->objectId;
        classNames << objectDetail->className;
        sizes << objectDetail->size;
        refCounts << objectDetail->references.size();
        locations << objectDetail->getFileLine();

        batchSize += 1;
        if (batchSize > 1000) {
            query.addBindValue(objectids);
            query.addBindValue(classNames);
            query.addBindValue(sizes);
            query.addBindValue(refCounts);
            query.addBindValue(locations);
            if (!query.execBatch()) {
                qDebug() << query.lastError();
            }
            objectids.clear();
            classNames.clear();
            sizes.clear();
            refCounts.clear();
            locations.clear();
            batchSize = 0;
        }
        ++iter;
    }
    query.addBindValue(objectids);
    query.addBindValue(classNames);
    query.addBindValue(sizes);
    query.addBindValue(refCounts);
    query.addBindValue(locations);
    if (!query.execBatch()) {
        qDebug() << query.lastError();
    }
    objectids.clear();
    classNames.clear();
    sizes.clear();
    refCounts.clear();
    locations.clear();
    batchSize = 0;
}


void RBKit::ObjectStore::addObject(RBKit::ObjectDetail *objectDetail)
{
    objectStore[objectDetail->objectId] = objectDetail;
    ++objectTypeCount[objectDetail->className];
}

void RBKit::ObjectStore::removeObject(quint64 key)
{
    RBKit::ObjectDetail *objectDetail = getObject(key);
    if(objectDetail != NULL) {
        quint32 oldCount = objectTypeCount[objectDetail->className];
        if(oldCount > 0) {
            oldCount -= 1;
        }
        objectTypeCount[objectDetail->className] = oldCount;
        delete objectDetail;
    }
    objectStore.remove(key);
}

void RBKit::ObjectStore::reset() {
    QHash<quint64, RBKit::ObjectDetail*>::iterator iter = objectStore.begin();
    while(iter != objectStore.end()) {
        delete iter.value();
        objectStore[iter.key()] = NULL;
        ++iter;
    }
    objectStore.clear();
    objectTypeCount.clear();
}


void RBKit::ObjectStore::updateObjectGeneration()
{
    QHash<quint64, ObjectDetail*>::const_iterator iter = objectStore.constBegin();
    while(iter != objectStore.constEnd()) {
        RBKit::ObjectDetail* detail = iter.value();
        detail->updateGeneration();
        ++iter;
    }
}

RBKit::ObjectDetail *RBKit::ObjectStore::getObject(quint64 key)
{
    QHash<quint64, RBKit::ObjectDetail*>::const_iterator i = objectStore.find(key);
    if(i != objectStore.end()) {
        return i.value();
    } else {
        return NULL;
    }
}

quint32 RBKit::ObjectStore::getObjectTypeCount(const QString &className)
{
    return objectTypeCount[className];
}

const quint32 RBKit::ObjectStore::liveObjectCount() const
{
    return objectStore.size();
}

const QVariantMap RBKit::ObjectStore::getObjectTypeCountMap()
{
    QVariantMap map;
    QHash<QString, quint32>::const_iterator typeIter;
    for(typeIter = objectTypeCount.begin()
        ; typeIter != objectTypeCount.end()
        ; typeIter++) {
        map[typeIter.key()] = typeIter.value();
    }
    return map;
}

std::list<QString> RBKit::ObjectStore::sort(int critirea) const
{
    QMap<QString, quint32> map;
    std::list<QString> classNames = objectTypeCount.keys().toStdList();
    Sorter s(this);
    classNames.sort(s);
    return classNames;
}

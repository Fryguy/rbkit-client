#ifndef OBJECTGENERATION_H
#define OBJECTGENERATION_H

#include <QMap>

class ObjectGeneration
{
    // objects that survive one GC
    QMap<QString, int> firstGeneration;
    // objects that survice two gcs
    QMap<QString, int> secondGeneration;
    // objects that survice 3 gcs
    QMap<QString, int> thirdGeneration;
    // objects that survice more than 3 gcs
    QMap<QString, int> oldGeneration;
public:
    ObjectGeneration();
};

#endif // OBJECTGENERATION_H

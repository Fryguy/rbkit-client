#ifndef OBJECTDETAIL_H
#define OBJECTDETAIL_H

#include <QString>

class ObjectDetail
{
    const QString *objectType;
    const int objectLine;
    const QString *fileLocation;
public:
    ObjectDetail(const QString *objectType);
};

#endif // OBJECTDETAIL_H

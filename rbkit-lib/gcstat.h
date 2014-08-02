#ifndef GCSTAT_H
#define GCSTAT_H

#include <map>
#include <string>
#include <QString>
#include "eventdata.h"
#include "subscriber.h"

class GcStat
{
    double timestamp;
    QString eventName;
    std::map<std::string, long> heapStat;
public:
    GcStat(std::map<std::string, long> dataMap, double timestamp, const QString eventName);
    GcStat(std::string eventName, MapStrMsgPackObj& parsedMap);
};

#endif // GCSTAT_H

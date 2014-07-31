#ifndef GCSTAT_H
#define GCSTAT_H

#include <map>
#include <string>
#include <QString>

class GcStat : public EventData
{
    std::map<std::string, long> heapStat;
public:
    GcStat(std::map<std::string, long>, double timestamp, const QString& eventName);
    GcStat(std::string& eventName, MapStrMsgPackObj& parsedMap);
};

#endif // GCSTAT_H

#include "gcstat.h"

GcStat::GcStat(std::map<std::string, long> heapStat, double timestamp, const QString& eventName)
{
    this->heapStat = heapStat;
    this->timestamp = timestamp;
    this->eventName = eventName;
}

GcStat::GcStat(std::string& eventName, MapStrMsgPackObj& parsedMap) {

}

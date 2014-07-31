#include "gcstat.h"

GcStat::GcStat(std::map<std::string, long> heapStat, double timestamp, const QString& eventName):
    heapStat(heapStat), timestamp(timestamp), eventName(eventName)
{
}

GcStat::GcStat(std::string& eventName, MapStrMsgPackObj& parsedMap) {

}

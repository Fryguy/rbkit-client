#ifndef GCSTAT_H
#define GCSTAT_H

#include <map>
#include <string>

class GcStat
{
    std::map<std::string, long> heapStat;
public:
    GcStat(std::map<std::string, long>);
};

#endif // GCSTAT_H

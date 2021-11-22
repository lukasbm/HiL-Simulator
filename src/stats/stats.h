#ifndef STATS_STATS_H_
#define STATS_STATS_H_

#include "json.hpp"
#include <omnetpp.h>
#include "../scheduler/Task_m.h"

using json = nlohmann::json;

class Stats {
public:
    static void finish();

    static void recordCpuUsage(const char* pcName, double val, omnetpp::simtime_t time, int core);
    static void recordTaskAssign(const char* pcName, Task* t, omnetpp::simtime_t time, int core);
    static void recordTaskRelease(const char* pcName, Task* t, omnetpp::simtime_t time, int core);
    static void recordTaskAdd(const char* pcName, Task* t, omnetpp::simtime_t time);
    static void recordTaskRun(const char* pcName, Task* t, omnetpp::simtime_t time);
    static void recordTaskCompleted(const char* pcName, Task* t, omnetpp::simtime_t time);

private:
    static json j;
};

#endif /* STATS_STATS_H_ */

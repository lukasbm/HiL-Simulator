#include "stats.h"
#include <iostream>
#include <fstream>

json Stats::j;

/**
 * completes the stat recording and writes results to output file.
 */
void Stats::finish() {
    std::ofstream o;
    o.open("results/output.json");
    o << j.dump(4) << std::endl;
    o.close();
}


void Stats::recordCpuUsage(const char* pcName, double val, omnetpp::simtime_t time, int core) {
    j[pcName]["cpuUsage"][std::to_string(core)].push_back({
       {"time", time.dbl()},
       {"value", val}
        });
}

void Stats::recordTaskAssign(const char* pcName, Task* t, omnetpp::simtime_t time, int core) {
    j[pcName]["tasks"][std::to_string(t->getId())]["assign"].push_back({
        {"time", time.dbl()},
        {"core", core}
        });
}

void Stats::recordTaskRelease(const char* pcName, Task* t, omnetpp::simtime_t time, int core) {
    j[pcName]["tasks"][std::to_string(t->getId())]["release"].push_back({
        {"time", time.dbl()},
        {"core", core}
        });
}

void Stats::recordTaskAdd(const char* pcName, Task* t, omnetpp::simtime_t time) {
    j[pcName]["tasks"][std::to_string(t->getId())]["name"] = t->getEmitterName();
    j[pcName]["tasks"][std::to_string(t->getId())]["add"] = time.dbl();
}

void Stats::recordTaskRun(const char* pcName, Task* t, omnetpp::simtime_t time) {
    j[pcName]["tasks"][std::to_string(t->getId())]["run"].push_back(time.dbl());
}

void Stats::recordTaskCompleted(const char* pcName, Task* t, omnetpp::simtime_t time) {
    j[pcName]["tasks"][std::to_string(t->getId())]["completed"] = time.dbl();
}

#ifndef SCHEDULER_ISCHEDULED_H_
#define SCHEDULER_ISCHEDULED_H_

#include <omnetpp.h>
using namespace omnetpp;

class IScheduled {
public:
    static cModule* findPC(cSimpleModule* caller);
    static void scheduleTask(cSimpleModule* caller, simtime_t processingTime);
    static void scheduleTask(cSimpleModule* caller, simtime_t processingTime, cMessage* msg);
    static bool messageFromScheduler(cSimpleModule* caller, cMessage* message);
};

#endif /* SCHEDULER_ISCHEDULED_H_ */

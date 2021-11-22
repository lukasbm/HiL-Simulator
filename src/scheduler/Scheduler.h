#ifndef SCHEDULER_SCHEDULER_H_
#define SCHEDULER_SCHEDULER_H_

#include <omnetpp.h>
#include "CPUCore.h"

using namespace omnetpp;

class Scheduler : public cSimpleModule {
private:
    cMessage* schedPing;
protected:
    int cores;
    int min_priority;
    int max_priority;
    std::vector<CPUCore*> cpu;

protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage* msg) override;
    virtual void finish() override;
    virtual ~Scheduler();

protected:
    /**
     * Runs the scheduling algorithm and returns the time when the next task is expected to be finished.
     *
     * Note: this time is not reliable, as there can be a another call to schedule() in the meantime.
     * Therefore if you use scheduleAt(schedule(), msg) you should call cancelEvent(msg) before invoking schedule() again.
     */
    virtual simtime_t schedule() = 0;

    /**
     * Adds a Task to the scheduler.
     * Note: it will only be scheduled once you call schedule()
     */
    virtual void add(Task* t) = 0;
};

#endif /* SCHEDULER_SCHEDULER_H_ */

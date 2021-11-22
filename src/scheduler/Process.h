#ifndef SCHEDULER_PROCESS_H_
#define SCHEDULER_PROCESS_H_

#include <omnetpp.h>
#include <queue>
#include "Task_m.h"

using namespace omnetpp;

/**
 * This class represents a process.
 * Allows to group together multiple modules to make them appear as one unit.
 * Acts as a proxy between the scheduler and the modules/tasks.
 */
class Process : public cSimpleModule {
private:
    std::queue<Task*> queue;
    cModule* sched;
    Task* current = nullptr;
protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage* msg) override;

    void nextTask();
};

#endif /* SCHEDULER_PROCESS_H_ */

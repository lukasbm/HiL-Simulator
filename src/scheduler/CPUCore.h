#ifndef SCHEDULER_CPUCORE_H_
#define SCHEDULER_CPUCORE_H_

#include "Task_m.h"
#include <omnetpp.h>

using namespace omnetpp;

enum State {
    IDLE,
    BUSY
};

/**
 * represents a CPU Core.
 *
 * Note: threads, cores and multi-processors are all treated as CPUCores here.
 */
class CPUCore : public cSimpleModule {
protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage* msg) override;

private:
    State state = State::IDLE;
    Task* current = nullptr;

    simtime_t cpuTime = SimTime(0, SIMTIME_MS);
    simtime_t lastTime = simTime();

    cMessage* usagePing = new cMessage("usagePing");

public:
    void assign(Task* t);
    void release();
    State getState();
    Task* getCurrent();
};

#endif /* SCHEDULER_CPUCORE_H_ */

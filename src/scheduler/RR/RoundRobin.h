#ifndef SCHEDULER_RR_H_
#define SCHEDULER_RR_H_

#include "../Scheduler.h"
#include <deque>
#include <vector>

class RoundRobin : public Scheduler {
private:
    double quantum;
    std::vector<std::deque<Task*>> queues; // global (cross cpu core) task queues
    simtime_t lastTime = simTime(); // last time the scheduler got called
protected:
    void add(Task* t) override;
    simtime_t schedule() override;
    virtual void initialize() override;
};

#endif /* SCHEDULER_RR_H_ */

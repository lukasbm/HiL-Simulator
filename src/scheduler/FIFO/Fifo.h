#ifndef SCHEDULER_FIFO_H_
#define SCHEDULER_FIFO_H_

#include "../Scheduler.h"
#include <deque>
#include <vector>

class Fifo : public Scheduler {
private:
    std::vector<std::deque<Task*>> queues; // global (cross cpu core) task queues
    simtime_t lastTime = simTime(); // last time the scheduler got called
protected:
    void add(Task* t) override;
    simtime_t schedule() override;
    virtual void initialize() override;
};

#endif /* SCHEDULER_FIFO_H_ */

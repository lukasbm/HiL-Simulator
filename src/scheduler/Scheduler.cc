#include "Scheduler.h"
#include "Task_m.h"
#include <vector>
#include "../stats/stats.h"

Register_Abstract_Class(Scheduler);

void Scheduler::initialize()
{
    this->schedPing = new cMessage("schedule");

    this->cores = par("cores").intValue();
    this->min_priority = par("min_priority").intValue();
    this->max_priority = par("max_priority").intValue();

    for (int i = 0; i < cores; i++) {
        cModule* m = getParentModule()->getSubmodule("cpuCores", i);
        CPUCore* c = check_and_cast<CPUCore*>(m);
        this->cpu.push_back(c);
    }
    EV_INFO << "STARTING SCHEDULER with " << this->cores << " cores" << endl;
}

void Scheduler::handleMessage(cMessage* msg) {
    if (msg->isSelfMessage()) { // signal task
        // rescheduling
        cancelEvent(schedPing);
        simtime_t schedTime = schedule();
        if (schedTime.dbl() >= 0 && (schedTime - simTime()).dbl() >= 0) {
            scheduleAt(schedTime, schedPing); // FIXME zwischen leeren queues und 0ms tasks unterscheiden
        }
    }
    else { // schedule task
        Task* task = check_and_cast<Task*>(msg);
        // update time
        int freq = par("frequency");
        int mfreq = par("measurement_frequency");
        double new_time = task->getTime() * ((double)mfreq / (double)freq);
        task->setTime(new_time);
        // check prio
        if (task->getPriority() < min_priority || task->getPriority() > max_priority) {
            throw cRuntimeError("invalid task priority");
        }
        // schedule it
        add(task);
        cancelEvent(schedPing);
        simtime_t schedTime = schedule();
        if (schedTime.dbl() >= 0 && (schedTime - simTime()).dbl() >= 0) {
            scheduleAt(schedTime, schedPing); // FIXME zwischen leeren queues und 0ms tasks unterscheiden
        }
    }
}

void Scheduler::finish() {
    Stats::finish();
}

Scheduler::~Scheduler() {
    cancelAndDelete(schedPing);
}

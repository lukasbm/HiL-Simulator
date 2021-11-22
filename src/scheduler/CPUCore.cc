#include "CPUCore.h"
#include "../stats/stats.h"

Define_Module(CPUCore);

void CPUCore::initialize() {
    scheduleAt(simTime() + par("probeInterval"), usagePing);
}

void CPUCore::handleMessage(cMessage* msg) {
    if (!msg->isSelfMessage()) throw cRuntimeError("invalid message");

    if (this->state == State::BUSY) cpuTime += simTime() - lastTime;

    double usage = cpuTime.dbl() / par("probeInterval").doubleValue();
    Stats::recordCpuUsage(getParentModule()->getFullName(), usage, simTime(), getIndex());

    // reset and reschedule
    cpuTime = SimTime(0, SIMTIME_MS);
    lastTime = simTime();
    scheduleAt(simTime() + par("probeInterval"), usagePing);
}

void CPUCore::assign(Task* t) {
    Enter_Method("CPUCore::assign()");
    if (this->state == State::BUSY) return;

    this->current = t;
    this->state = State::BUSY;
    Stats::recordTaskAssign(getParentModule()->getFullName(), t, simTime(), getIndex());

    lastTime = simTime();
}

void CPUCore::release() {
    Enter_Method("CPUCore::release()");
    if (this->state == State::IDLE) return;

    this->state = State::IDLE;
    Stats::recordTaskRelease(getParentModule()->getFullName(), this->current, simTime(), getIndex());
    this->current = nullptr;

    cpuTime += simTime() - lastTime;
}

State CPUCore::getState() {
    Enter_Method_Silent();
    return this->state;
}

Task* CPUCore::getCurrent() {
    Enter_Method_Silent();
    return this->current;
}

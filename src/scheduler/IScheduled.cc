#include "IScheduled.h"

#include "Task_m.h"

cModule* IScheduled::findPC(cSimpleModule* caller) {
    cModule* pc = caller;
    if (caller->par("usingPCsubtype").boolValue() == true) {
        do
        {
            pc = pc->getParentModule();
        } while (!strcasestr(pc->getName(), "pc"));
    }
    else {
        while (pc->getSubmodule("pc") == nullptr) {
            pc = pc->getParentModule();
        }
        pc = pc->getSubmodule("pc");
    }
    return pc;
}

void IScheduled::scheduleTask(cSimpleModule* caller, simtime_t processingTime, cMessage* notiMsg) {
    Task* msg = new Task("task");
    msg->setTime(processingTime.dbl());
    int prio = caller->par("priority").intValue();
    msg->setPriority(prio);
    msg->setEmitterName(caller->getFullName());

    cModule* pc = findPC(caller);
    bool use_sched = caller->par("schedule").boolValue();
    if (use_sched) {
        int process = caller->par("process").intValue();
        if (process == -1) { // send to scheduler
            cModule* scheduler = pc->getSubmodule("scheduler");
            caller->sendDirect(msg, scheduler, "in");
        }
        else { // send to process proxy
            cModule* proxy = pc->getSubmodule("processes", process);
            caller->sendDirect(msg, proxy, "in");
        }
    }
    else {
        caller->scheduleAt(simTime() + processingTime, notiMsg);
    }
}

void IScheduled::scheduleTask(cSimpleModule* caller, simtime_t processingTime) {
    scheduleTask(caller, processingTime, new cMessage("IScheduled::processingTimer"));
}

bool IScheduled::messageFromScheduler(cSimpleModule* caller, cMessage* message) {
    int process = caller->par("process");
    cModule* pc = findPC(caller);
    cModule* scheduler = pc->getSubmodule("scheduler");
    cModule* proxy = pc->getSubmodule("processes", process);

    return (scheduler != nullptr && message->getSenderModuleId() == scheduler->getId()) ||
        (proxy != nullptr && message->getSenderModuleId() == proxy->getId()) ||
        message->isSelfMessage();
}

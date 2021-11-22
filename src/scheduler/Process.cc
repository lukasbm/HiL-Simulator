#include "Process.h"

Define_Module(Process);

void Process::initialize()
{
    sched = getParentModule()->getSubmodule("scheduler");
}

void Process::handleMessage(cMessage* msg) {
    Task* task = dynamic_cast<Task*>(msg);
    if (task == nullptr) throw cRuntimeError("Process only handles Tasks");

    if (task->getSenderModuleId() == sched->getId()) { // message from scheduler
        sendDirect(current, current->getSenderModule(), "scheduler"); //forward back to module
        current = nullptr;
        nextTask();
    }
    else { // message from module
        queue.push(task);
        if (current == nullptr) nextTask(); // execute if no other task is currently running in this process
    }
}

void Process::nextTask() {
    if (!queue.empty()) {
        Task* next = queue.front();
        current = next->dup();
        EV_INFO << "SEND " << next->getSenderModule()->getFullName() << " TO SCHEDULER FROM PROCESS " << this->getFullName() << endl;
        sendDirect(next, sched, "in");
        queue.pop();
    }
}

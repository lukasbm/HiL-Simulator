#include "CustPacketDelayer.h"

#include "inet/common/ModuleAccess.h"
#include <string.h>

#include "scheduler/IScheduled.h"

Define_Module(CustPacketDelayer);

using namespace inet::queueing;

void CustPacketDelayer::initialize(int stage)
{
    PassivePacketSinkBase::initialize(stage);

    if (stage == INITSTAGE_LOCAL) {
        inputGate = gate("in");
        producer = findConnectedModule<IActivePacketSource>(inputGate);
        outputGate = gate("out");
        consumer = findConnectedModule<IPassivePacketSink>(outputGate);
    }
}

void CustPacketDelayer::handleMessage(cMessage* message)
{
    if (IScheduled::messageFromScheduler(this, message)) {
        pushOrSendPacket(nextPackets.front(), outputGate, consumer);
        nextPackets.pop();
    }
    else
        throw cRuntimeError("Unknown message");
}

void CustPacketDelayer::pushPacket(Packet* packet, cGate* gate)
{
    Enter_Method("pushPacket");
    EV_INFO << "Delaying packet " << packet->getName() << "." << endl;
    take(packet);
    packet->setArrival(getId(), inputGate->getId(), simTime());
    nextPackets.push(packet);
    // send 'task' with delay to scheduler
    IScheduled::scheduleTask(this, par("delay"));

    numProcessedPackets++;
    processedTotalLength += packet->getTotalLength();
    updateDisplayString();
}

void CustPacketDelayer::handleCanPushPacket(cGate* gate)
{
    Enter_Method("handleCanPushPacket");
    if (producer != nullptr)
        producer->handleCanPushPacket(inputGate);
}

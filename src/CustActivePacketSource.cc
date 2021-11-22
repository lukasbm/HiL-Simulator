#include "CustActivePacketSource.h"

#include "inet/common/ModuleAccess.h"
#include "inet/common/Simsignals.h"
#include "inet/common/TimeTag_m.h"
#include "inet/queueing/common/LabelsTag_m.h"
#include <string.h>

#include "scheduler/IScheduled.h"

Define_Module(CustActivePacketSource);

void CustActivePacketSource::initialize(int stage)
{
    CustPacketSourceBase::initialize(stage);

    if (stage == INITSTAGE_LOCAL) {
        outputGate = gate("out");
        consumer = findConnectedModule<IPassivePacketSink>(outputGate);
        productionTimer = new cMessage("ProductionTimer");
    }
    else if (stage == INITSTAGE_QUEUEING) {
        checkPushPacketSupport(outputGate);
        if (consumer == nullptr && !productionTimer->isScheduled())
            scheduleProductionTimer();
    }

}

void CustActivePacketSource::handleMessage(cMessage* message)
{
    if (message == productionTimer) {
        if (consumer == nullptr || consumer->canPushSomePacket(outputGate->getPathEndGate())) {
            scheduleProductionTimer();
            producePacket();
        }
    }
    else if (IScheduled::messageFromScheduler(this, message)) {
        scheduleAt(simTime().dbl() + par("productionInterval").doubleValue(), productionTimer);
    }
    else 
        throw cRuntimeError("Unknown message");
}

void CustActivePacketSource::scheduleProductionTimer()
{
    IScheduled::scheduleTask(this, par("processingTime"));
}

void CustActivePacketSource::producePacket()
{
    auto packet = createPacket();
    EV_INFO << "Producing packet " << packet->getName() << "." << endl;
    packet->setTimestamp();
    emit(packetCreatedSignal, packet);
    long id = -1;
    auto data = packet->peekData();
    auto labels = data->getAllTags<LabelsTag>();
    for (auto& label : labels)
    {
        id = std::atol(label.getTag()->getLabels(1));
    }
    emit(packetIDSignal, id);

    pushOrSendPacket(packet, outputGate, consumer);
    updateDisplayString();
}

void CustActivePacketSource::handleCanPushPacket(cGate* gate)
{
    Enter_Method("handleCanPushPacket");
    if (gate->getPathStartGate() == outputGate) {
        scheduleProductionTimer();
    }
}

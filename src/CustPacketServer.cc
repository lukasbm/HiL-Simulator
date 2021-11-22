#include "CustPacketServer.h"

#include "inet/common/Simsignals.h"
#include <string.h>
#include "scheduler/IScheduled.h"

using namespace inet;
using namespace inet::queueing;

Define_Module(CustPacketServer);

void CustPacketServer::initialize(int stage)
{
    PacketServerBase::initialize(stage);
}

void CustPacketServer::handleMessage(cMessage* message)
{
    if (IScheduled::messageFromScheduler(this, message)) {
        if (packet != nullptr) endProcessingPacket();
        if (canStartProcessingPacket()) {
            startProcessingPacket();
            scheduleProcessingTimer();
        }
        updateDisplayString();
    }
    else
        throw cRuntimeError("Unknown message");
}

void CustPacketServer::scheduleProcessingTimer()
{
    simtime_t processingTime = par("processingTime");
    auto processingBitrate = bps(par("processingBitrate"));
    processingTime += s(packet->getTotalLength() / processingBitrate).get();
    // schedule it
    IScheduled::scheduleTask(this, processingTime);
}

bool CustPacketServer::canStartProcessingPacket()
{
    return provider->canPopSomePacket(inputGate->getPathStartGate()) &&
        consumer->canPushSomePacket(outputGate->getPathEndGate());
}

void CustPacketServer::startProcessingPacket()
{
    packet = provider->popPacket(inputGate->getPathStartGate());
    take(packet);
    packet->setArrival(getId(), inputGate->getId(), simTime());
    EV_INFO << "Processing packet " << packet->getName() << " started." << endl;
}

void CustPacketServer::endProcessingPacket()
{
    EV_INFO << "Processing packet " << packet->getName() << " ended.\n";
    processedTotalLength += packet->getDataLength();
    emit(packetServedSignal, packet);
    pushOrSendPacket(packet, outputGate, consumer);
    numProcessedPackets++;
    packet = nullptr;
}

void CustPacketServer::handleCanPushPacket(cGate* gate)
{
    Enter_Method("handleCanPushPacket");
    if (canStartProcessingPacket()) {
        startProcessingPacket();
        scheduleProcessingTimer();
    }
}

void CustPacketServer::handleCanPopPacket(cGate* gate)
{
    Enter_Method("handleCanPopPacket");
    if (canStartProcessingPacket()) {
        startProcessingPacket();
        scheduleProcessingTimer();
    }
}

const char* CustPacketServer::resolveDirective(char directive) const
{
    return "processing";
}

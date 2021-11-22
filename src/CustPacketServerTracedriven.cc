#include "inet/common/Simsignals.h"
#include "CustPacketServerTracedriven.h"

#include "scheduler/IScheduled.h"

using namespace inet;
using namespace inet::queueing;

Define_Module(CustPacketServerTracedriven);

void CustPacketServerTracedriven::initialize(int stage)
{
    PacketServerBase::initialize(stage);
    if (stage == INITSTAGE_LOCAL) {
        std::vector<std::string> timesStr = cStringTokenizer(par("processingTimeTrace")).asVector();
        for (auto i : timesStr) {
            simtime_t t = simtime_t::parse(i.c_str());
            timesPar.push_back(t);
        }
    }
}

void CustPacketServerTracedriven::handleMessage(cMessage* message)
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

void CustPacketServerTracedriven::scheduleProcessingTimer()
{
    simtime_t processingTime = par("processingTime");
    auto processingBitrate = bps(par("processingBitrate"));
    processingTime += s(packet->getTotalLength() / processingBitrate).get();
    processingTime += timesPar[J];
    if (J < timesPar.size() - 1) {
        J++;
    }
    else {
        J = 0;
    }
    // send 'task' with processing time to scheduler
    IScheduled::scheduleTask(this, processingTime);
}

bool CustPacketServerTracedriven::canStartProcessingPacket()
{
    return provider->canPopSomePacket(inputGate->getPathStartGate()) &&
        consumer->canPushSomePacket(outputGate->getPathEndGate());
}

void CustPacketServerTracedriven::startProcessingPacket()
{
    packet = provider->popPacket(inputGate->getPathStartGate());
    take(packet);
    packet->setArrival(getId(), inputGate->getId(), simTime());
    EV_INFO << "Processing packet " << packet->getName() << " started." << endl;
}

void CustPacketServerTracedriven::endProcessingPacket()
{
    EV_INFO << "Processing packet " << packet->getName() << " ended.\n";
    processedTotalLength += packet->getDataLength();
    emit(packetServedSignal, packet);
    pushOrSendPacket(packet, outputGate, consumer);
    numProcessedPackets++;
    packet = nullptr;
}

void CustPacketServerTracedriven::handleCanPushPacket(cGate* gate)
{
    Enter_Method("handleCanPushPacket");
    if (canStartProcessingPacket()) {
        startProcessingPacket();
        scheduleProcessingTimer();
    }
}

void CustPacketServerTracedriven::handleCanPopPacket(cGate* gate)
{
    Enter_Method("handleCanPopPacket");
    if (canStartProcessingPacket()) {
        startProcessingPacket();
        scheduleProcessingTimer();
    }
}

const char* CustPacketServerTracedriven::resolveDirective(char directive) const
{
    return "processing";
}








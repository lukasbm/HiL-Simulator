#include "CustPacketServerWithBufferTimeAndTimeStamp.h"

#include "inet/common/Simsignals.h"
#include "inet/queueing/server/PacketServer.h"
#include "inet/common/TimeTag_m.h"
#include "inet/queueing/common/LabelsTag_m.h"
#include <string.h>

#include "scheduler/IScheduled.h"

Define_Module(CustPacketServerWithBufferTimeAndTimeStamp);

using namespace inet;
using namespace inet::queueing;

void CustPacketServerWithBufferTimeAndTimeStamp::initialize(int stage)
{
    PacketServerBase::initialize(stage);
}

void CustPacketServerWithBufferTimeAndTimeStamp::handleMessage(cMessage* message)
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

void CustPacketServerWithBufferTimeAndTimeStamp::scheduleProcessingTimer()
{
    simtime_t processingTime = par("processingTime");
    auto processingBitrate = bps(par("processingBitrate"));
    processingTime += s(packet->getTotalLength() / processingBitrate).get();
    processingTime += par("bufferInterval");

    // send 'task' with processing time to scheduler
    IScheduled::scheduleTask(this, processingTime);

    emit(schedulingDiffSignal, packet->getTimestamp() + processingTime - simTime());
    emit(creatingDiffSignal, packet->getCreationTime() - packet->getTimestamp());
}

bool CustPacketServerWithBufferTimeAndTimeStamp::canStartProcessingPacket()
{
    return provider->canPopSomePacket(inputGate->getPathStartGate()) &&
        consumer->canPushSomePacket(outputGate->getPathEndGate());
}

void CustPacketServerWithBufferTimeAndTimeStamp::startProcessingPacket()
{
    packet = provider->popPacket(inputGate->getPathStartGate());
    take(packet);

    // reading packet tags
    auto packetLength = b(0);
    long packetID = -1;

    auto data = packet->peekData();
    auto creationTimes = data->getAllTags<CreationTimeTag>();
    for (auto& creationTime : creationTimes)
    {
        auto time = creationTime.getTag()->getCreationTime();
        packet->setTimestamp(time);
    }

    auto labels = data->getAllTags<LabelsTag>();
    for (auto& label : labels)
    {
        auto length = label.getTag()->getLabels(0);
        packetLength = b(std::atoi(length));
        auto id = label.getTag()->getLabels(1);
        packetID = std::atol(id);
    }

    packet->setArrival(getId(), inputGate->getId(), simTime());
    EV_INFO << "Processing packet " << packet->getName() << " started." << endl;
}

void CustPacketServerWithBufferTimeAndTimeStamp::endProcessingPacket()
{
    EV_INFO << "Processing packet " << packet->getName() << " ended.\n";
    processedTotalLength += packet->getDataLength();
    emit(packetServedSignal, packet);

    emit(endToEndDelaySignal, simTime() - packet->getTimestamp());
    long id = -1;
    auto data = packet->peekData();
    auto labels = data->getAllTags<LabelsTag>();
    for (auto& label : labels)
    {
        id = std::atol(label.getTag()->getLabels(1));
    }
    emit(packetIDSignal, id);

    pushOrSendPacket(packet, outputGate, consumer);
    numProcessedPackets++;
    packet = nullptr;
}

void CustPacketServerWithBufferTimeAndTimeStamp::handleCanPushPacket(cGate* gate)
{
    Enter_Method("handleCanPushPacket");
    if (canStartProcessingPacket()) {
        startProcessingPacket();
        scheduleProcessingTimer();
    }
}

void CustPacketServerWithBufferTimeAndTimeStamp::handleCanPopPacket(cGate* gate)
{
    Enter_Method("handleCanPopPacket");
    if (canStartProcessingPacket()) {
        startProcessingPacket();
        scheduleProcessingTimer();
    }
}

const char* CustPacketServerWithBufferTimeAndTimeStamp::resolveDirective(char directive) const
{
    return "processing";
}

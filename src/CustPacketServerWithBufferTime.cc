#include "CustPacketServerWithBufferTime.h"
#include "inet/common/Simsignals.h"
#include "inet/queueing/server/PacketServer.h"

Define_Module(CustPacketServerWithBufferTime);

using namespace inet;
using namespace inet::queueing;

void CustPacketServerWithBufferTime::initialize(int stage)
{
    PacketServerBase::initialize(stage);
    if (stage == INITSTAGE_LOCAL)
        processingTimer = new cMessage("ProcessingTimer");
}

void CustPacketServerWithBufferTime::handleMessage(cMessage* message)
{
    if (simTime() >= par("bufferInterval").doubleValue())
    {
        if (message == processingTimer) {
            endProcessingPacket();
            if (canStartProcessingPacket()) {
                startProcessingPacket();
                scheduleProcessingTimer();
            }
            updateDisplayString();
        }
        else
            throw cRuntimeError("Unknown message");
    }
}

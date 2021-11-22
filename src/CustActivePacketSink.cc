#include "CustActivePacketSink.h"

Define_Module(CustActivePacketSink);

using namespace inet;
using namespace inet::queueing;

void CustActivePacketSink::initialize(int stage)
{
    ActivePacketSink::initialize(stage);
    if (stage == INITSTAGE_LOCAL) {
        endToEndDelayVec.setName("End-to-End Delay");
    }
}

void CustActivePacketSink::collectPacket()
{

    auto packet = provider->popPacket(inputGate->getPathStartGate());
    EV_INFO << "Collecting packet " << packet->getName() << "." << endl;
    numProcessedPackets++;
    processedTotalLength += packet->getDataLength();
    updateDisplayString();
    simtime_t eed = simTime() - packet->getCreationTime();
    endToEndDelayVec.record(eed);
    dropPacket(packet, OTHER_PACKET_DROP);

    // TODO - Generated method body
}


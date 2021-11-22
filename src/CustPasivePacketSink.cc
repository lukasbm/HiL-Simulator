#include "CustPasivePacketSink.h"

Define_Module(CustPasivePacketSink);

using namespace inet;
using namespace inet::queueing;


void CustPasivePacketSink::initialize(int stage)
{
    PassivePacketSink::initialize(stage);
    if (stage == INITSTAGE_LOCAL) {
        endToEndDelayVec.setName("End-to-End Delay");
    }
}

void CustPasivePacketSink::consumePacket(Packet* packet)
{

    EV_INFO << "Consuming packet " << packet->getName() << "." << endl;
    numProcessedPackets++;
    processedTotalLength += packet->getDataLength();
    updateDisplayString();
    simtime_t now = simTime();
    simtime_t pctCreation = packet->getCreationTime();
    simtime_t eed = now - pctCreation;
    endToEndDelayVec.record(eed);
    dropPacket(packet, OTHER_PACKET_DROP);


}

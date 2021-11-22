#include "PacketDropper.h"

Define_Module(PacketDropper);

void PacketDropper::initialize()
{
    // TODO - Generated method body
    numProcessedPackets = 0;
    numDroppedPackets = 0;

    dropTimeParam = par("dropTime");
    dropPacketIndexParam = par("packetNumber");

    inputGate = gate("in");
    outputGate = gate("out");
}

void PacketDropper::handleMessage(cMessage* msg)
{
    auto packet = check_and_cast<Packet*>(msg);
    // TODO - Generated method body
    if (packet->getByteLength() > 200)
    {
        numProcessedPackets++;

        packetPushVector.recordWithTimestamp(simTime(), packet->getByteLength());
        // If number of packet is equal to an index, drop that packet
        if (numProcessedPackets == dropPacketIndexParam || (dropTimeParam > 0 && simTime() >= dropTimeParam && numDroppedPackets == 0))
        {
            packetDropVector.recordWithTimestamp(simTime(), packet->getByteLength());
            numDroppedPackets++;
            take(packet);
            dropAndDelete(packet);
            return;
        }
        else
        {

            packetPassedVector.recordWithTimestamp(simTime(), packet->getByteLength());
            send(msg, outputGate);
        }
    }
    else
    {
        send(msg, outputGate);
    }


}

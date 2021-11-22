#ifndef __AD_HIL_ETHERNET_PACKETDROPPER_H_
#define __AD_HIL_ETHERNET_PACKETDROPPER_H_

#include <omnetpp.h>
#include "inet/common/packet/Packet.h"

using namespace omnetpp;
using namespace inet;

/**
 * TODO - Generated class
 */
class PacketDropper : public cSimpleModule
{
protected:
    cGate* inputGate = nullptr;
    cGate* outputGate = nullptr;
    int numProcessedPackets = 0;
    int numDroppedPackets = 0;

    simtime_t dropTimeParam;
    int dropPacketIndexParam = 0;

    cOutVector packetPushVector;
    cOutVector packetDropVector;
    cOutVector packetPassedVector;

    virtual void initialize();
    virtual void handleMessage(cMessage* msg);
};

#endif

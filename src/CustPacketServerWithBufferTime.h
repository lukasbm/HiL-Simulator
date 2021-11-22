#ifndef __AD_HIL_ETHERNET_CUSTPACKETSERVERWITHBUFFERTIME_H_
#define __AD_HIL_ETHERNET_CUSTPACKETSERVERWITHBUFFERTIME_H_

#include <omnetpp.h>
#include "inet/common/packet/Packet.h"
#include "inet/queueing/server/PacketServer.h"
#include "inet/queueing/base/PacketServerBase.h"

using namespace omnetpp;
using namespace inet;
using namespace inet::queueing;

class CustPacketServerWithBufferTime : public PacketServer
{
protected:
    cMessage* processingTimer = nullptr;
    Packet* packet = nullptr;

protected:
    virtual void initialize(int stage) override;
    virtual void handleMessage(cMessage* message) override;
};

#endif // ifndef __INET_PACKETSERVER_H

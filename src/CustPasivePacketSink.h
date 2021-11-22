#ifndef __AD_HIL_ETHERNET_CUSTPASIVEPACKETSINK_H_
#define __AD_HIL_ETHERNET_CUSTPASIVEPACKETSINK_H_

#include <omnetpp.h>

#include "inet/queueing/sink/PassivePacketSink.h"


using namespace omnetpp;
using namespace inet;
using namespace inet::queueing;

/**
 * TODO - Generated class
 */
class CustPasivePacketSink : public PassivePacketSink
{
protected:
    cOutVector endToEndDelayVec;
    virtual void initialize(int stage) override;
    virtual void consumePacket(Packet* packet) override;
};

#endif

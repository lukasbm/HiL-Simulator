#ifndef __AD_HIL_ETHERNET_CUSTACTIVEPACKETSINK_H_
#define __AD_HIL_ETHERNET_CUSTACTIVEPACKETSINK_H_

#include <omnetpp.h>
#include "inet/queueing/sink/ActivePacketSink.h"


using namespace omnetpp;
using namespace inet;
using namespace inet::queueing;

/**
 * TODO - Generated class
 */
class CustActivePacketSink : public ActivePacketSink
{
protected:
    cOutVector endToEndDelayVec;
    virtual void initialize(int stage) override;
    virtual void collectPacket() override;

};

#endif

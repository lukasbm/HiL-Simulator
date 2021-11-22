#ifndef __AD_HIL_ETHERNET_CUSTPACKETDELAYER_H_
#define __AD_HIL_ETHERNET_CUSTPACKETDELAYER_H_

#include <omnetpp.h>
#include "inet/queueing/base/PassivePacketSinkBase.h"
#include "inet/queueing/contract/IActivePacketSource.h"
#include <queue>

using namespace omnetpp;
using namespace inet;
using namespace inet::queueing;

class CustPacketDelayer : public PassivePacketSinkBase, public IActivePacketSource
{
protected:
    cGate* inputGate = nullptr;
    IActivePacketSource* producer = nullptr;

    cGate* outputGate = nullptr;
    IPassivePacketSink* consumer = nullptr;

    std::queue<Packet*> nextPackets;

protected:
    virtual void initialize(int stage) override;
    virtual void handleMessage(cMessage* message) override;

public:
    virtual IPassivePacketSink* getConsumer(cGate* gate) override { return consumer; }

    virtual bool supportsPushPacket(cGate* gate) const override { return true; }
    virtual bool supportsPopPacket(cGate* gate) const override { return true; }

    virtual void pushPacket(Packet* packet, cGate* gate) override;

    virtual void handleCanPushPacket(cGate* gate) override;
};


#endif

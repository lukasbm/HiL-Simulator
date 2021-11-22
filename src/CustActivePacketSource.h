#ifndef __INET_CUSTACTIVEPACKETSOURCE_H
#define __INET_CUSTACTIVEPACKETSOURCE_H

#include <omnetpp.h>
#include "CustPacketSourceBase.h"
#include "inet/queueing/contract/IActivePacketSource.h"

using namespace omnetpp;
using namespace inet;
using namespace inet::queueing;

class CustActivePacketSource : public CustPacketSourceBase, public IActivePacketSource
{
private:
    simsignal_t packetCreatedSignal = cComponent::registerSignal("packetCreated");
    simsignal_t packetIDSignal = cComponent::registerSignal("packetID");

protected:
    cGate* outputGate = nullptr;
    IPassivePacketSink* consumer = nullptr;
    cMessage* productionTimer = nullptr;

protected:
    virtual void initialize(int stage) override;
    virtual void handleMessage(cMessage* message) override;

    virtual void scheduleProductionTimer();
    virtual void producePacket();

public:
    virtual ~CustActivePacketSource() { cancelAndDelete(productionTimer); }

    virtual IPassivePacketSink* getConsumer(cGate* gate) override { return consumer; }

    virtual bool supportsPushPacket(cGate* gate) const override { return outputGate == gate; }
    virtual bool supportsPopPacket(cGate* gate) const override { return false; }

    virtual void handleCanPushPacket(cGate* gate) override;
};


#endif // ifndef __INET_CUSTACTIVEPACKETSOURCE_H


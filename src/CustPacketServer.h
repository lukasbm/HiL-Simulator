#ifndef __AD_HIL_ETHERNET_CUSTPACKETSERVER_H_
#define __AD_HIL_ETHERNET_CUSTPACKETSERVER_H_

#include <omnetpp.h>
#include "inet/queueing/base/PacketServerBase.h"

using namespace omnetpp;
using namespace inet;
using namespace inet::queueing;

class CustPacketServer : public PacketServerBase
{
protected:
    Packet* packet = nullptr;

protected:
    virtual void initialize(int stage) override;
    virtual void handleMessage(cMessage* message) override;
    virtual void scheduleProcessingTimer();
    virtual bool canStartProcessingPacket();
    virtual void startProcessingPacket();
    virtual void endProcessingPacket();

public:
    virtual IPassivePacketSource* getProvider(cGate* gate) override { return provider; }
    virtual IPassivePacketSink* getConsumer(cGate* gate) override { return consumer; }

    virtual void handleCanPushPacket(cGate* gate) override;
    virtual void handleCanPopPacket(cGate* gate) override;

    virtual const char* resolveDirective(char directive) const override;
};

#endif // ifndef __INET_CUSTPACKETSERVER_H

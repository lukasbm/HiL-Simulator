#ifndef __AD_HIL_ETHERNET_CUSTPACKETSERVERTRACEDRIVEN_H_
#define __AD_HIL_ETHERNET_CUSTPACKETSERVERTRACEDRIVEN_H_

#include <omnetpp.h>
#include "inet/queueing/base/PacketServerBase.h"

using namespace omnetpp;
using namespace inet;
using namespace inet::queueing;

class CustPacketServerTracedriven : public PacketServerBase
{
protected:
    Packet* packet = nullptr;
    int J = 0;
    std::vector<std::string> timesStr;
    std::vector<simtime_t> timesPar;

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

#endif // ifndef __AD_HIL_ETHERNET_CUSTPACKETSERVERTRACEDRIVEN_H_


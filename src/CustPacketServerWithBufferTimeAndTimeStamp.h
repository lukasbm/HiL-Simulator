#ifndef __AD_HIL_ETHERNET_CUSTPACKETSERVERWITHBUFFERTIMEANDTIMESTAMP_H_
#define __AD_HIL_ETHERNET_CUSTPACKETSERVERWITHBUFFERTIMEANDTIMESTAMP_H_

#include <omnetpp.h>
#include "inet/common/packet/Packet.h"
#include "inet/queueing/server/PacketServer.h"
#include "inet/queueing/base/PacketServerBase.h"

using namespace omnetpp;
using namespace inet;
using namespace inet::queueing;

class CustPacketServerWithBufferTimeAndTimeStamp : public PacketServer
{
private:
    simsignal_t endToEndDelaySignal = cComponent::registerSignal("endToEndDelay");
    simsignal_t packetIDSignal = cComponent::registerSignal("packetID");
    simsignal_t schedulingDiffSignal = cComponent::registerSignal("schedulingDiff");
    simsignal_t creatingDiffSignal = cComponent::registerSignal("creatingDiff");

protected:
    cOutVector endToEndDelayVec;
    Packet* packet = nullptr;

protected:
    virtual void initialize(int stage) override;
    virtual void handleMessage(cMessage* message) override;
    virtual void scheduleProcessingTimer();
    virtual bool canStartProcessingPacket();
    virtual void startProcessingPacket();
    virtual void endProcessingPacket();
public:
    virtual void handleCanPushPacket(cGate* gate) override;
    virtual void handleCanPopPacket(cGate* gate) override;

    virtual const char* resolveDirective(char directive) const override;

};

#endif // ifndef __INET_PACKETSERVER_H


#ifndef __AD_HIL_ETHERNET_PACKETPROCESSOR_H_
#define __AD_HIL_ETHERNET_PACKETPROCESSOR_H_

#include <omnetpp.h>
#include "inet/common/Units.h"
#include "inet/common/packet/ChunkQueue.h"
#include "inet/common/packet/Packet.h"

using namespace omnetpp;
using namespace inet::units::values;
using namespace inet;

/**
 * TODO - Generated class
 */
class PacketProcessor : public cSimpleModule
{
protected:
    inet::ChunkQueue queue; // stores application data for merging
    cPar* packetLengthParameter = nullptr;
    //b dataCapacity = b();
    const char* displayStringTextFormat = nullptr;
    int numProcessedTcpPackets = 0;
    int numMergedPckts = 0;
    b processedTotalLength = b(-1);
    int numPushedPackets = -1;
    int numPoppedPackets = -1;
    int numCurrentPackets = -1;
    int numCreatedPackets = -1;
    cOutVector packetpoptime;
    virtual void initialize();
    virtual void handleMessage(cMessage* msg);

    //  virtual void emit(simsignal_t signal, cObject *object, cObject *details = nullptr) override;

    virtual void updateDisplayString();
};

#endif

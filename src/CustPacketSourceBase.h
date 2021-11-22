#ifndef __INET_PACKETSOURCEBASE_H
#define __INET_PACKETSOURCEBASE_H

#include "inet/queueing/base/PacketProcessorBase.h"
#include "inet/queueing/base/PacketSourceBase.h"

using namespace omnetpp;
using namespace inet;
using namespace inet::queueing;

class CustPacketSourceBase : public PacketProcessorBase
{
protected:
    const char* packetNameFormat = nullptr;
    const char* packetRepresentation = nullptr;
    cPar* packetLengthParameter = nullptr;
    cPar* packetDataParameter = nullptr;
    long packetCounter = -1;

protected:
    virtual void initialize(int stage) override;

    virtual const char* createPacketName(const Ptr<const Chunk>& data) const;
    virtual Ptr<Chunk> createPacketContent() const;
    virtual Packet* createPacket();
};


#endif // ifndef __INET_PACKETSOURCEBASE_H


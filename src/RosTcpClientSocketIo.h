#ifndef __AD_HIL_ETHERNET_ROSTCPCLIENTSOCKETIO_H_
#define __AD_HIL_ETHERNET_ROSTCPCLIENTSOCKETIO_H_

#include <omnetpp.h>
#include "inet/applications/tcpapp/TcpClientSocketIo.h"

//using namespace omnetpp;

namespace inet {

    /**
     * TODO - Generated class
     */
    class RosTcpClientSocketIo : public TcpClientSocketIo
    {
    protected:
        simtime_t startTime;
        virtual void initialize() override;
        virtual void handleMessage(cMessage* message) override;
    };

}
#endif

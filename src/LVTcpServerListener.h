#ifndef __AD_HIL_ETHERNET_LVTCPSERVERLISTENER_H_
#define __AD_HIL_ETHERNET_LVTCPSERVERLISTENER_H_

#include <omnetpp.h>
#include "inet/applications/base/ApplicationBase.h"
#include "inet/applications/tcpapp/TcpServerSocketIo.h"
#include "inet/transportlayer/contract/tcp/TcpSocket.h"
#include "inet/applications/tcpapp/TcpServerListener.h"

//using namespace omnetpp;

namespace inet {
    /**
     * TODO - Generated class
     */
    class LVTcpServerListener : public TcpServerListener
    {
    protected:
    public:

        virtual void socketAvailable(TcpSocket* socket, TcpAvailableInfo* availableInfo) override;
    };

}

#endif

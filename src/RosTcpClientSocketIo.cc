#include "RosTcpClientSocketIo.h"

namespace inet {

    Define_Module(RosTcpClientSocketIo);

    void RosTcpClientSocketIo::initialize()
    {
        // TODO - Generated method body
        TcpClientSocketIo::initialize();
        startTime = par("startTime");
        // Open the connection at start up
        cMessage* timeoutMsg = new cMessage("OpenConnection");
        scheduleAt(startTime, timeoutMsg);
    }

    void RosTcpClientSocketIo::handleMessage(cMessage* message)
    {
        if (message->arrivedOn("socketIn")) {
            ASSERT(socket && socket->belongsToSocket(message));
            socket->processMessage(message);
        }
        else if (message->arrivedOn("trafficIn")) {
            if (socket == nullptr)
                open();
            socket->send(check_and_cast<Packet*>(message));
        }
        else if (message->isSelfMessage()) {
            if (socket == nullptr)
                open();
        }
        else
            throw cRuntimeError("Unknown message");
    }

}

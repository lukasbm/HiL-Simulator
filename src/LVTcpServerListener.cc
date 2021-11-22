#include "LVTcpServerListener.h"

namespace inet {
    Define_Module(LVTcpServerListener);

    void LVTcpServerListener::socketAvailable(TcpSocket* socket, TcpAvailableInfo* availableInfo)
    {
        const char* serverConnectionModuleType = par("serverConnectionModuleType");
        cModuleType* moduleType = cModuleType::get(serverConnectionModuleType);
        cModule* submodule = getParentModule()->getSubmodule("connection", 0);
        int submoduleIndex = submodule == nullptr ? 0 : submodule->getVectorSize();
        auto connection = moduleType->create("connection", getParentModule(), submoduleIndex + 1, submoduleIndex);
        connection->finalizeParameters();
        connection->buildInside();
        connection->callInitialize();

        auto dispatcher = check_and_cast<cSimpleModule*>(gate("socketIn")->getPathStartGate()->getOwnerModule());

        //Connect the packetOut gate of connection to the serverApp
        //auto serverApp = check_and_cast<cModule *>(dispatcher->gate("in",1)->getPathStartGate()->getOwnerModule());
        auto serverApp = getParentModule();
        serverApp->setGateSize("packetOut", serverApp->gateSize("packetOut") + 1);
        connection->gate("packetOut")->connectTo(serverApp->gate("packetOut", 0));

        //Connect the packetOut gate of serverApp to the serverHost
        auto serverHost = serverApp->getParentModule();
        serverApp->gate("packetOut", 0)->connectTo(serverHost->gate("tcpAppOut", serverApp->getIndex()));

        dispatcher->setGateSize("in", dispatcher->gateSize("in") + 1);
        dispatcher->setGateSize("out", dispatcher->gateSize("out") + 1);
        connection->gate("socketOut")->connectTo(dispatcher->gate("in", dispatcher->gateSize("in") - 1));
        dispatcher->gate("out", dispatcher->gateSize("out") - 1)->connectTo(connection->gate("socketIn"));
        auto serverSocketIo = check_and_cast<TcpServerSocketIo*>(connection->gate("socketIn")->getPathEndGate()->getOwnerModule());
        serverSocketIo->acceptSocket(availableInfo);
        connectionSet.insert(serverSocketIo);

    }


}

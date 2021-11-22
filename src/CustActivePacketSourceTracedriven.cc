#include "CustActivePacketSourceTracedriven.h"

#include "inet/common/ModuleAccess.h"
#include "inet/common/Simsignals.h"
#include "inet/common/TimeTag_m.h"
#include "inet/queueing/common/LabelsTag_m.h"

#include "scheduler/IScheduled.h"

Define_Module(CustActivePacketSourceTracedriven);

void CustActivePacketSourceTracedriven::initialize(int stage)
{
    CustPacketSourceBase::initialize(stage);

    if (stage == INITSTAGE_LOCAL) {
        outputGate = gate("out");
        consumer = findConnectedModule<IPassivePacketSink>(outputGate);
        std::vector<std::string> timesStr = cStringTokenizer(par("productionInterval")).asVector();
        for (auto i : timesStr) {
            simtime_t t = simtime_t::parse(i.c_str());
            timesPar.push_back(t);
        }
        productionTimer = new cMessage("ProductionTimer");
    }
    else if (stage == INITSTAGE_QUEUEING) {
        checkPushPacketSupport(outputGate);
        if (consumer == nullptr && !productionTimer->isScheduled())
            scheduleProductionTimer();
    }
}

void CustActivePacketSourceTracedriven::handleMessage(cMessage* message)
{
    if (message == productionTimer) {
        if (consumer == nullptr || consumer->canPushSomePacket(outputGate->getPathEndGate())) {
            scheduleProductionTimer();
            producePacket();
        }
    }
    else if (IScheduled::messageFromScheduler(this, message)) {
        scheduleAt(simTime().dbl() + timesPar[J].dbl(), productionTimer);
        if (J < timesPar.size() - 1) {
            J++;
        }
        else {
            J = 0;
        }
    }
    else
        throw cRuntimeError("Unknown message");
}

void CustActivePacketSourceTracedriven::scheduleProductionTimer()
{
    IScheduled::scheduleTask(this, par("processingTime"));
}

void CustActivePacketSourceTracedriven::producePacket()
{
    auto packet = createPacket();

    packet->setTimestamp();
    EV_INFO << "Producing packet " << packet->getName() << " in CustActivePacketSourceTracedriven Original Timestamp is " << packet->getTimestamp() << endl;
    //emit(packetCreatedSignal, packet);
    long id = -1;
    auto data = packet->peekData();
    auto labels = data->getAllTags<LabelsTag>();
    for (auto& label : labels)
    {
        id = std::atol(label.getTag()->getLabels(1));
    }
    emit(packetIDSignal, id);

    pushOrSendPacket(packet, outputGate, consumer);
    updateDisplayString();
}

void CustActivePacketSourceTracedriven::handleCanPushPacket(cGate* gate)
{
    Enter_Method("handleCanPushPacket");
    if (gate->getPathStartGate() == outputGate && !productionTimer->isScheduled()) {
        scheduleProductionTimer();
        //producePacket();
    }
}


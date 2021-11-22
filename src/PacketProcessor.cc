#include "PacketProcessor.h"
#include "inet/common/Simsignals.h"
#include "inet/common/StringFormat.h"
#include "inet/common/TimeTag_m.h"
#include "inet/queueing/common/LabelsTag_m.h"

Define_Module(PacketProcessor);

void PacketProcessor::initialize()
{
    // TODO - Generated method body
    // Based on the sensor type, there will be a max packet size
    // Packets will be merged till this size is achieved.
    //Then the merged packet will be sent to the next module
    //dataCapacity = b(par("dataCapacity"));
    displayStringTextFormat = par("displayStringTextFormat");
    packetLengthParameter = &par("packetLength");
    numPushedPackets = 0;
    numPoppedPackets = 0;
    numCurrentPackets = 0;
    numCreatedPackets = 0;
    WATCH(numPushedPackets);
    WATCH(numPoppedPackets);
    WATCH(numProcessedTcpPackets);
    WATCH(numMergedPckts);
    WATCH(processedTotalLength);
}

void PacketProcessor::handleMessage(cMessage* msg)
{
    // TODO - Generated method body

    auto packet = check_and_cast<Packet*>(msg);
    auto packetLength = b(packetLengthParameter->intValue());
    long packetID = -1;

    // reading packet tags
    auto data = packet->peekData();
    auto creationTimes = data->getAllTags<CreationTimeTag>();
    for (auto& creationTime : creationTimes)
    {
        auto time = creationTime.getTag()->getCreationTime();
        packet->setTimestamp(time);
    }

    auto labels = data->getAllTags<LabelsTag>();
    for (auto& label : labels)
    {
        auto length = label.getTag()->getLabels(0);
        packetLength = b(std::atoi(length));
        auto id = label.getTag()->getLabels(1);
        packetID = std::atol(id);
    }



    //  auto packettotallength = packet->getTotalLength();
    //  auto  chunks = (queue.getLength().size()/ packetLength);
    if (queue.getLength() < packetLength)
    {
        queue.push(packet->peekData());
        numProcessedTcpPackets++;
        numCurrentPackets++;
        numPushedPackets++;
        //  emit(packetPushedSignal, msg);
        updateDisplayString();
    }


    // if(queue.getLength() == packetLength || packetLength < B(1460)) // If packet length is less than mss then each mss will have a packet
    if (queue.getLength() >= packetLength)// || packetLength < B(1460))
    {
        while (queue.getLength() >= packetLength) {
            //auto dataPacket = new Packet("OrgData",queue.pop(queue.getLength())); // create new segment with the merged content
            auto dataPacket = new Packet("OrgData", queue.pop(packetLength));
            send(dataPacket, "out");

            numMergedPckts++;
            numCreatedPackets++;
            numPoppedPackets++;

            processedTotalLength = processedTotalLength + dataPacket->getTotalLength();

        }
        numCurrentPackets--;
        //    emit(packetPoppedSignal, dataPacket);
        updateDisplayString();
    }

    delete packet;

}

/*void PacketProcessor::emit(simsignal_t signal, cObject *object, cObject *details)
{
    if (signal == packetPushedSignal)
        numPushedPackets++;
    else if (signal == packetPoppedSignal)
        numPoppedPackets++;
    cSimpleModule::emit(signal, object, details);
}*/

void PacketProcessor::updateDisplayString()
{
    if (getEnvir()->isGUI()) {
        auto text = StringFormat::formatString(displayStringTextFormat, [&](char directive) {
            static std::string result;
            switch (directive) {
            case 'p':
                result = std::to_string(numCurrentPackets);
                break;
            case 'u':
                result = std::to_string(numPushedPackets);
                break;
            case 'o':
                result = std::to_string(numPoppedPackets);
                break;
            case 'r':
                result = std::to_string(numCreatedPackets);
                break;
            default:
                throw cRuntimeError("Unknown directive: %c", directive);
            }
            return result.c_str();
            });
        getDisplayString().setTagArg("t", 0, text);
    }
}

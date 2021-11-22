#include "inet/applications/base/ApplicationPacket_m.h"

#include "inet/common/packet/chunk/BitCountChunk.h"
#include "inet/common/packet/chunk/BitsChunk.h"
#include "inet/common/packet/chunk/ByteCountChunk.h"
#include "inet/common/packet/chunk/BytesChunk.h"
#include "inet/common/Simsignals.h"
#include "inet/common/TimeTag_m.h"
#include "inet/queueing/common/LabelsTag_m.h"
#include "CustPacketSourceBase.h"


void CustPacketSourceBase::initialize(int stage)
{
    PacketProcessorBase::initialize(stage);
    if (stage == INITSTAGE_LOCAL) {
        packetNameFormat = par("packetNameFormat");
        packetRepresentation = par("packetRepresentation");
        packetLengthParameter = &par("packetLength");
        packetDataParameter = &par("packetData");
        packetCounter = 0;
    }
}

const char* CustPacketSourceBase::createPacketName(const Ptr<const Chunk>& data) const
{
    return StringFormat::formatString(packetNameFormat, [&](char directive) {
        static std::string result;
        switch (directive) {
        case 'n':
            result = getFullName();
            break;
        case 'p':
            result = getFullPath();
            break;
        case 'c':
            result = std::to_string(numProcessedPackets);
            break;
        case 'l':
            result = data->getChunkLength().str();
            break;
        case 'd':
            if (auto byteCountChunk = dynamicPtrCast<const ByteCountChunk>(data))
                result = std::to_string(byteCountChunk->getData());
            else if (auto bitCountChunk = dynamicPtrCast<const BitCountChunk>(data))
                result = std::to_string(bitCountChunk->getData());
            break;
        case 't':
            result = simTime().str();
            break;
        case 'e':
            result = std::to_string(getSimulation()->getEventNumber());
            break;
        default:
            throw cRuntimeError("Unknown directive: %c", directive);
        }
        return result.c_str();
        });
}

Ptr<Chunk> CustPacketSourceBase::createPacketContent() const
{
    auto packetLength = b(packetLengthParameter->intValue());
    auto packetData = packetDataParameter->intValue();
    if (!strcmp(packetRepresentation, "bitCount"))
        return packetData == -1 ? makeShared<BitCountChunk>(packetLength) : makeShared<BitCountChunk>(packetLength, packetData);
    else if (!strcmp(packetRepresentation, "bits")) {
        static int total = 0;
        const auto& packetContent = makeShared<BitsChunk>();
        std::vector<bool> bits;
        bits.resize(b(packetLength).get());
        for (int i = 0; i < (int)bits.size(); i++)
            bits[i] = packetData == -1 ? (total + i) % 2 == 0 : packetData;
        total += bits.size();
        packetContent->setBits(bits);
        return packetContent;
    }
    else if (!strcmp(packetRepresentation, "byteCount"))
        return packetData == -1 ? makeShared<ByteCountChunk>(packetLength) : makeShared<ByteCountChunk>(packetLength, packetData);
    else if (!strcmp(packetRepresentation, "bytes")) {
        static int total = 0;
        const auto& packetContent = makeShared<BytesChunk>();
        std::vector<uint8_t> bytes;
        bytes.resize(B(packetLength).get());
        for (int i = 0; i < (int)bytes.size(); i++)
            bytes[i] = packetData == -1 ? (total + i) % 256 : packetData;
        total += bytes.size();
        packetContent->setBytes(bytes);
        return packetContent;
    }
    else if (!strcmp(packetRepresentation, "applicationPacket")) {
        const auto& packetContent = makeShared<ApplicationPacket>();
        packetContent->setChunkLength(B(packetLength));
        packetContent->setSequenceNumber(numProcessedPackets);
        return packetContent;
    }
    else
        throw cRuntimeError("Unknown representation");
}

Packet* CustPacketSourceBase::createPacket()
{
    auto packetContent = createPacketContent();

    // add tags to packet content
    packetContent->addTag<CreationTimeTag>()->setCreationTime(simTime());

    auto label = packetContent->addTag<LabelsTag>();
    label->setLabelsArraySize(2);
    // tag for packet length
    label->setLabels(0, std::to_string(b(packetContent->getChunkLength()).get()).c_str());
    // tag for packet id
    label->setLabels(1, std::to_string(packetCounter).c_str());
    packetCounter++;

    auto packetName = createPacketName(packetContent);
    auto packet = new Packet(packetName, packetContent);
    numProcessedPackets++;
    processedTotalLength += packet->getDataLength();
    emit(packetCreatedSignal, packet);
    return packet;
}



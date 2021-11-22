package ad_hil_ethernet;

import inet.queueing.common.PacketDelayer;
import inet.queueing.common.PacketGate;
import inet.queueing.contract.IPassivePacketSink;
import inet.queueing.queue.DropTailQueue;
import inet.queueing.server.PacketServer;
import inet.queueing.sink.PassivePacketSink;

module ClusterLabVIEWTracedriven
{
    parameters:
        int numSensorStreams = default(1);
        volatile double bufferInterval @unit(s) = default(1s);

        @figure[submodules];

        @display("bgb=3738.5625,904.875");
    gates:
        input in[];
        output out[];
    submodules:
        packetProcessor[numSensorStreams]: PacketProcessor {
            @display("p=551.65625,289.71875");
        }
        dropTailQueueLV0[numSensorStreams]: DropTailQueue {
            @display("p=893.98376,678.0991");
        }
        packetServer[numSensorStreams]: CustPacketServerTracedriven {
            @display("p=1195.6687,287.84616");
        }
        dropTailQueueLV1[numSensorStreams]: DropTailQueue {
            @display("p=1583.1539,678.0991");
        }
        packetServer1[numSensorStreams]: CustPacketServerTracedriven {
            @display("p=1931.8906,287.84616");
        }
        passivePacketSink[numSensorStreams]: PassivePacketSink {
            @display("p=3257.6436,678.0991");
        }
        custPacketServerWithBufferTimeAndTimeStamp[numSensorStreams]: CustPacketServerWithBufferTimeAndTimeStamp {
            @display("p=2845.2485,287.84616");
        }
        dropTailQueueLV2[numSensorStreams]: DropTailQueue {
            @display("p=2360.892,678.0991");
        }
    connections allowunconnected:
        for i=0..numSensorStreams-1 {
            in++ --> packetProcessor[i].in;
            packetProcessor[i].out --> dropTailQueueLV0[i].in;
            dropTailQueueLV0[i].out --> packetServer[i].in;
            packetServer[i].out --> dropTailQueueLV1[i].in;
            dropTailQueueLV1[i].out --> packetServer1[i].in;
            packetServer1[i].out --> dropTailQueueLV2[i].in;
            dropTailQueueLV2[i].out --> custPacketServerWithBufferTimeAndTimeStamp[i].in;
            custPacketServerWithBufferTimeAndTimeStamp[i].out --> passivePacketSink[i].in;
        }
}

package ad_hil_ethernet;

import inet.queueing.common.PacketGate;
import inet.queueing.queue.DropTailQueue;
import inet.queueing.server.PacketServer;
import inet.queueing.sink.PassivePacketSink;

module ClusterSink
{
    parameters:
        int numSensorStreams = default(1);
        volatile double collectionInterval @unit(s) = default(0.04s);
        volatile double bufferInterval @unit(s) = default(1s);

        @figure[submodules];

        @display("bgb=2051.4038,238.21875");
    gates:
        //For output to Client server
        input in[];

    submodules:
        packetProcessor[numSensorStreams]: PacketProcessor {
            @display("p=154,129.8");
        }
        dropTailQueue_LV1[numSensorStreams]: DropTailQueue {
            @display("p=442.40625,129.31876");
        }
        PacketServer[numSensorStreams]: CustPacketServer {
            @display("p=782.71875,129.31876");
        }
        PacketDelayer[numSensorStreams]: CustPacketDelayer {
            @display("p=1094.4451,129.31876");
        }
        dropTailQueue_LV2[numSensorStreams]: DropTailQueue {
            @display("p=1411.6163,129.31876");
        }
        passivePacketSink[numSensorStreams]: PassivePacketSink {
            @display("p=1967.6625,128.4375");
        }
        custPacketServerWithBufferTimeAndTimeStamp[numSensorStreams]: CustPacketServerWithBufferTimeAndTimeStamp {
            @display("p=1672.2562,128.4375");
        }
    connections allowunconnected:
        for i=0..numSensorStreams-1 {
            in++ --> packetProcessor[i].in;
            packetProcessor[i].out --> dropTailQueue_LV1[i].in;
            dropTailQueue_LV1[i].out --> PacketServer[i].in;
            PacketServer[i].out --> PacketDelayer[i].in;
            PacketDelayer[i].out --> dropTailQueue_LV2[i].in;
            dropTailQueue_LV2[i].out --> custPacketServerWithBufferTimeAndTimeStamp[i].in;
            custPacketServerWithBufferTimeAndTimeStamp[i].out --> passivePacketSink[i].in;
        }
}

package ad_hil_ethernet;

import inet.queueing.common.PacketDelayer;
import inet.queueing.common.PacketGate;
import inet.queueing.contract.IPassivePacketSink;
import inet.queueing.queue.DropTailQueue;
import inet.queueing.server.PacketServer;
import inet.queueing.sink.PassivePacketSink;

module ClusterLabVIEW
{
    parameters:
        int numSensorStreams = default(1);
        volatile double bufferInterval @unit(s) = default(1s);

        @figure[submodules];

        @display("bgb=2705.2437,585.0575");
    gates:
        //For in-&output to Client server
        input in[];
        output out[];

    submodules:
        packetProcessor[numSensorStreams]: PacketProcessor {
            @display("p=396.07874,129.4375");
        }
        dropTailQueue1[numSensorStreams]: DropTailQueue {
            @display("p=774.03625,365.01373");
        }
        packetServer[numSensorStreams]: CustPacketServer {
            @display("p=1133.8724,129.4375");
        }
        dropTailQueue2[numSensorStreams]: DropTailQueue {
            @display("p=1434.1675,365.01373");
        }
        passivePacketSink[numSensorStreams]: PassivePacketSink {
            @display("p=2392.005,365.01373");
        }
        custPacketServerWithBufferTimeAndTimeStamp[numSensorStreams]: CustPacketServerWithBufferTimeAndTimeStamp {
            @display("p=2073.5886,129.4375");
        }
    connections allowunconnected:
        for i=0..numSensorStreams-1 {
            in++ --> packetProcessor[i].in;
            packetProcessor[i].out --> dropTailQueue1[i].in;
            dropTailQueue1[i].out --> packetServer[i].in;
            packetServer[i].out --> dropTailQueue2[i].in;
            dropTailQueue2[i].out --> custPacketServerWithBufferTimeAndTimeStamp[i].in;
            custPacketServerWithBufferTimeAndTimeStamp[i].out --> passivePacketSink[i].in;
        }
}

package ad_hil_ethernet;

import inet.queueing.queue.DropTailQueue;

// This module encapsulates all the Sensor Data source(ActiveSource) for a similar kind of sensor stream.
// This makes it easy to manage parametrs for all the similar sensor streams data generators.
module SensorClusterROSApplication
{
    parameters:
        int numSensors = default(1);
        volatile double streamingDuration @unit(s) = default(1ms); // Duration of data production and streaming : To mimick RosReplayer
        volatile int streamFileSize @unit(b) = default(0b); // Duration of streaming
        volatile double productionIntervl @unit(s) = default(1ms); // elapsed time between subsequent packets pushed to the connected packet consumer
        string packetRep @units("bitCount","bits","byteCount","bytes","applicationPacket") = default("bits"); // determines the chunk of the packet data
        volatile int packetLen @unit(b) = default(1B);
        volatile int packData = default(-1);
        volatile bool useSingleOut = default(false);
        //bool useDelayChannel = default(false);
        @figure[submodules];

    gates:
        //For output to Client server
        output out[];

    submodules:
        dropTailQueue[numSensors]: DropTailQueue {
            @display("p=156.92308,108.46154");
        }
        packetServer[numSensors]: CustPacketServerWithBufferTimeAndTimeStamp {
            @display("p=233.84616,40.769234");
        }
        dropTailQueue1[numSensors]: DropTailQueue {
            @display("p=310,108.46154");
        }
        packetServer1[numSensors]: CustPacketServerWithBufferTimeAndTimeStamp {
            @display("p=374.6154,40.769234");
        }
        packetDelayer2[numSensors]: CustPacketDelayer {
            @display("p=733.8462,108.46154");
        }
        activePacketSource[numSensors]: CustActivePacketSource {
            @display("p=86.15385,40.769234");
        }
        dropTailQueue2[numSensors]: DropTailQueue {
            @display("p=444.6154,108.46154");
        }
        packetServer2[numSensors]: CustPacketServerWithBufferTimeAndTimeStamp {
            @display("p=519.2308,40.769234");
        }
        dropTailQueue3[numSensors]: DropTailQueue {
            @display("p=592.30774,108.46154");
        }
        packetServer3[numSensors]: CustPacketServerWithBufferTimeAndTimeStamp {
            @display("p=661.5385,40.769234");
        }
    connections allowunconnected:
        for i=0..numSensors-1, if !useSingleOut {
            dropTailQueue[i].out --> packetServer[i].in;
            packetDelayer2[i].out --> out++;
            dropTailQueue3[i].out --> packetServer3[i].in;
            dropTailQueue2[i].out --> packetServer2[i].in;
            activePacketSource[i].out --> dropTailQueue[i].in;
            packetServer[i].out --> dropTailQueue1[i].in;
            dropTailQueue1[i].out --> packetServer1[i].in;
            packetServer1[i].out --> dropTailQueue2[i].in;
            packetServer2[i].out --> dropTailQueue3[i].in;
            packetServer3[i].out --> packetDelayer2[i].in;
        }
}

package ad_hil_ethernet;
import inet.queueing.common.PacketDelayer;
import inet.queueing.queue.DropTailQueue;
import inet.queueing.server.PacketServer;

// This module encapsulates all the Sensor Data source(ActiveSource) for a similar kind of sensor stream.
// This makes it easy to manage parametrs for all the similar sensor streams data generators.
module SensorClusterROSApplicationTracedriven
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

        @figure[submodules];
        @display("bgb=1190.7693,360");
    gates:
        //For output to Client server
        output out[];

    submodules:
        activePacketSource[numSensors]: CustActivePacketSource {
            @display("p=105,46");
        }
        packetDelayer[numSensors]: CustPacketServerTracedriven {
            @display("p=474.6154,247.69232");
        }
        dropTailQueue_ROS2[numSensors]: DropTailQueue {
            @display("p=557.6923,142.3077");
        }
        packetServer[numSensors]: CustPacketServerTracedriven {
            @display("p=641.5385,45.384617");
        }
        packetDelayer1[numSensors]: CustPacketServerTracedriven {
            @display("p=842.30774,247.69232");
        }
        dropTailQueue_ROS3[numSensors]: DropTailQueue {
            @display("p=733.07697,142.3077");
        }
        packetServer1[numSensors]: CustPacketServerTracedriven {
            @display("p=983.07697,45.384617");
        }
        packetDelayer2[numSensors]: CustPacketDelayer {
            @display("p=1096.1539,247.69232");
        }
        dropTailQueue_ROS1[numSensors]: DropTailQueue {
            @display("p=385.38464,142.3077");
        }
        dropTailQueue_ROS4[numSensors]: DropTailQueue {
            @display("p=914.6154,142.3077");
        }
        dropTailQueue_ROSReplayer[numSensors]: DropTailQueue {
            @display("p=192.3077,143.84616");
        }
        packetServerROSReplayer[numSensors]: CustPacketServerTracedriven {
            @display("p=286.15387,45.384617");
        }
    connections allowunconnected:
        for i=0..numSensors-1, if !useSingleOut {
            activePacketSource[i].out --> dropTailQueue_ROSReplayer[i].in;
            dropTailQueue_ROSReplayer[i].out --> packetServerROSReplayer[i].in;
            packetServerROSReplayer[i].out --> dropTailQueue_ROS1[i].in;
            dropTailQueue_ROS1[i].out --> packetDelayer[i].in;
            packetDelayer[i].out --> dropTailQueue_ROS2[i].in;
            dropTailQueue_ROS2[i].out --> packetServer[i].in;
            packetServer[i].out --> dropTailQueue_ROS3[i].in;
            dropTailQueue_ROS3[i].out --> packetDelayer1[i].in;
            packetDelayer1[i].out --> dropTailQueue_ROS4[i].in;
            dropTailQueue_ROS4[i].out --> packetServer1[i].in;
            packetServer1[i].out --> packetDelayer2[i].in;
            packetDelayer2[i].out --> out++;
        }




}

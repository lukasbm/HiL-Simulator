package ad_hil_ethernet.simulations.Test_for_verification;

import ad_hil_ethernet.ClusterLabVIEW;
import inet.networklayer.configurator.ipv4.Ipv4NetworkConfigurator;
import inet.node.inet.StandardHost;
import ned.DatarateChannel;
import ned.DelayChannel;
import inet.node.ethernet.Eth1G;
import inet.queueing.queue.DropTailQueue;

import ad_hil_ethernet.SensorClusterROSApplication;
import ad_hil_ethernet.RosApplicationLayerHost;

import ad_hil_ethernet.PC;

network easy_verification
{
    types:
	    // NOTE: instances of extended PC type have to have a pc in their name
        module PC1 extends PC
        {
            parameters:
                int numIMU = default(0);
                int nos = numIMU; // Number of sensors: This number will decide the number of TCP client and server apps
                string packetRepresentation @units("bitCount","bits","byteCount","bytes","applicationPacket") = default("bits");
                @display("bgb=432,213");
            submodules:
                rosDataIMU: SensorClusterROSApplication {
                    parameters:
                        numSensors = numIMU;
                        packetRep = packetRepresentation;
                        packData = intuniform(0, 1);
                        @display("p=242,100;i=device/mainframe");
                }
                dropTailQueue: DropTailQueue {
                    @display("p=340.23672,99.408295");
                }
            connections allowunconnected:
                rosDataIMU.out++ --> dropTailQueue.in;
        }

    submodules:
        pc1: PC1 {
            @display("p=100,100");
        }
}


network easy_verification_2streams
{
    types:
        module PC1 extends PC
        {
            parameters:
                int numIMU = default(0);
                int nos = numIMU; // Number of sensors: This number will decide the number of TCP client and server apps
                string packetRepresentation @units("bitCount","bits","byteCount","bytes","applicationPacket") = default("bits");
                @display("bgb=432,213");
            submodules:
                rosDataIMU: SensorClusterROSApplication {
                    parameters:
                        numSensors = numIMU;
                        packetRep = packetRepresentation;
                        packData = intuniform(0, 1);
                        @display("p=240,61;i=device/mainframe");
                }
                rosDataIMU2: SensorClusterROSApplication {
                    parameters:
                        numSensors = numIMU;
                        packetRep = packetRepresentation;
                        packData = intuniform(0, 1);
                        @display("p=240,127;i=device/mainframe");
                }
                dropTailQueue: DropTailQueue {
                    @display("p=353,61");
                }
                dropTailQueue2: DropTailQueue {
                    @display("p=353,127");
                }
            connections allowunconnected:
                rosDataIMU.out++ --> dropTailQueue.in;
                rosDataIMU2.out++ --> dropTailQueue2.in;
        }

    submodules:
        pc1: PC1 {
            @display("p=100,100");
        }
}

package ad_hil_ethernet.simulations.MRGen_for_validation_PC;

import inet.networklayer.configurator.ipv4.Ipv4NetworkConfigurator;
import inet.node.inet.StandardHost;
import ned.DatarateChannel;
import ned.DelayChannel;
import inet.node.ethernet.Eth1G;
import inet.queueing.queue.DropTailQueue;

import ad_hil_ethernet.ClusterLabVIEW;
import ad_hil_ethernet.SensorClusterROSApplication;
import ad_hil_ethernet.SensorClusterROSApplicationTracedriven;
import ad_hil_ethernet.ClusterLabVIEWTracedriven;
import ad_hil_ethernet.RosApplicationLayerHost;
import ad_hil_ethernet.LVApplicationLayerHost;

import ad_hil_ethernet.PC;

network MRGen_CAN_stream_Tracedriven
{
    types:
        channel C extends Eth1G
        {
            length = default(3m);
            ber = default(0);
            per = default(0);
        }

        module RosPC extends PC
        {
            parameters:
                string packetRepresentation @units("bitCount","bits","byteCount","bytes","applicationPacket") = default("bits");
                int numIMU = default(0);
                int nos = numIMU;
                @display("bgb=438,280");
            gates:
                output out[];
            submodules:
                rosDataIMU: SensorClusterROSApplicationTracedriven {
                    parameters:
                        numSensors = numIMU;
                        packetRep = packetRepresentation;
                        packData = intuniform(0, 1);
                        @display("p=49,222;i=device/mainframe");
                }
                client: RosApplicationLayerHost {
                    parameters:
                        numApps = nos;
                        @display("p=240,222;i=device/mainframe");
                }
                server: LVApplicationLayerHost {
                    parameters:
                        numApps = nos;
                        @display("p=381,222;i=device/mainframe");
                }
            connections:
                for i=0..numIMU-1 {
                    rosDataIMU.out++ --> client.tcpAppIn++;
                    client.ethg++ <--> C <--> server.ethg++;
                    server.tcpAppOut++ --> out++;
                }
        }

        module LabViewPC extends PC
        {
            parameters:
                string packetRepresentation @units("bitCount","bits","byteCount","bytes","applicationPacket") = default("bits");
                int numIMU = default(0);
                int nos = numIMU;
                @display("bgb=275,283");
            gates:
                input in[];
            submodules:
                sinkIMU: ClusterLabVIEWTracedriven {
                    @display("p=47.429996,219.48");
                }
            connections:
                for i=0..numIMU-1 {
                    in++ --> sinkIMU.in++;
                }
        }

    submodules:
        pc1: RosPC {

            @display("p=40,53");
        }
        pc2: LabViewPC {

            @display("p=120,53");
        }
        configurator: Ipv4NetworkConfigurator {
            parameters:
                @display("p=82,113;is=s");
        }
    connections:
        pc1.out++ --> pc2.in++;
}

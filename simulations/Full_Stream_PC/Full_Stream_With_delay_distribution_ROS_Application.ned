package ad_hil_ethernet.simulations.Full_Stream_PC;

import inet.networklayer.configurator.ipv4.Ipv4NetworkConfigurator;
import inet.node.inet.StandardHost;
import ned.DatarateChannel;
import ned.DelayChannel;
import inet.node.ethernet.Eth40G;
import inet.queueing.queue.DropTailQueue;

import ad_hil_ethernet.SensorClusterROSApplication;
import ad_hil_ethernet.ClusterSink;
import ad_hil_ethernet.RosApplicationLayerHost;
import ad_hil_ethernet.LVApplicationLayerHost;

import ad_hil_ethernet.PC;

network Full_Stream_With_delay_distribution_ROS_Application
{
    types:
        channel C extends Eth40G
        {
            length = default(3m);
            ber = default(0);
            per = default(0);
        }

        module RosPC extends PC
        {
            parameters:
                string packetRepresentation @units("bitCount","bits","byteCount","bytes","applicationPacket") = default("bits");
                int numRadar = default(0);
                int numIMU = default(0);
                int numCamera = default(0);
                int nos = numRadar+numIMU+numCamera;
                @display("bgb=629,271");
            gates:
                output out[];
            submodules:
                rosDataRadar: SensorClusterROSApplication {
                    parameters:
                        numSensors = numRadar;
                        packetRep = packetRepresentation;
                        packData = intuniform(0, 1);
                        @display("p=281,50;i=device/mainframe");
                }
                rosDataCamera: SensorClusterROSApplication {
                    parameters:
                        numSensors = numCamera;
                        packetRep = packetRepresentation;
                        packData = intuniform(0, 1);
                        @display("p=281,136;i=device/mainframe");
                }
                rosDataIMU: SensorClusterROSApplication {
                    parameters:
                        numSensors = numIMU;
                        packetRep = packetRepresentation;
                        packData = intuniform(0, 1);
                        @display("p=281,208;i=device/mainframe");
                }
                client: RosApplicationLayerHost {
                    parameters:
                        numApps = nos;
                        @display("p=424,136;i=device/mainframe");
                }
                server: LVApplicationLayerHost {
                    parameters:
                        numApps = nos;
                        @display("p=511,136;i=device/mainframe");
                }
                configurator: Ipv4NetworkConfigurator {
                    @display("p=122,154;is=s");
                }
            connections:
                client.ethg++ <--> C <--> server.ethg++;

                for i=0..numRadar-1 {
                    rosDataRadar.out++ --> client.tcpAppIn++;
                    server.tcpAppOut++ --> out++;
                }
                for i=0..numIMU-1 {
                    rosDataIMU.out++ --> client.tcpAppIn++;
                    server.tcpAppOut++ --> out++;
                }
                for i=0..numCamera-1 {
                    rosDataCamera.out++ --> client.tcpAppIn++;
                    server.tcpAppOut++ --> out++;
                }
        }

        module LabViewPC extends PC
        {
            parameters:
                int numRadar = default(0);
                int numIMU = default(0);
                int numCamera = default(0);
                @display("bgb=451,213");
            gates:
                input in[];
            submodules:
                sinkCam: ClusterSink {
                    parameters:
                        @display("p=306,86;i=device/memory");
                        numSensorStreams = numCamera;
                }
                sinkRad: ClusterSink {
                    parameters:
                        @display("p=248,86;i=device/memory");
                        numSensorStreams = numRadar;
                }
                sinkIMU: ClusterSink {
                    parameters:
                        @display("p=358,86;i=device/memory");
                        numSensorStreams = numIMU;
                }
                configurator: Ipv4NetworkConfigurator {
                    @display("p=122,154;is=s");
                }
            connections:
                for i=0..numRadar-1 {
                    in++ --> sinkRad.in++;
                }
                for i=0..numIMU-1 {
                    in++ --> sinkIMU.in++;
                }
                for i=0..numCamera-1 {
                    in++ --> sinkCam.in++;
                }
        }

    submodules:
        configurator: Ipv4NetworkConfigurator {
            @display("p=122,154;is=s");
        }
        pc1: RosPC {
            @display("p=62,94;is=s");

        }
        pc2: LabViewPC {
            @display("p=180,94;is=s");
        }

    connections:
        pc1.out++ --> pc2.in++;
}

package ad_hil_ethernet.simulations.Full_Stream_With_delay_distribution_ROS_Application;

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

        module PC1 extends PC
        {
            parameters:
                string packetRepresentation @units("bitCount","bits","byteCount","bytes","applicationPacket") = default("bits");
                // Distribution of different sensors
                int numRadar = default(0);
                int numIMU = default(0);
                int numCamera = default(0);
                int nos = numRadar+numIMU+numCamera; // Number of sesnsors: This number will decide the number of TCP client and server apps
            gates:
                inout g;

            submodules:
                rosDataRadar: SensorClusterROSApplication {
                    parameters:
                        numSensors = numRadar;
                        packetRep = packetRepresentation;
                        packData = intuniform(0, 1);
                        //streamingDuration = 10s;
                        //streamFileSize = 11400kB;//22800B;
                        //productionIntervl = simTime() > 0ms ? 70ms : 1ms;
                        //packetLen = 65050B;
                        @display("p=155,50;i=device/mainframe");
                }

                rosDataCamera: SensorClusterROSApplication {
                    parameters:
                        numSensors = numCamera;
                        packetRep = packetRepresentation;
                        packData = intuniform(0, 1);
                        //streamingDuration = 10s;
                        //streamFileSize = 600MB;//1200MB;
                        //productionIntervl = simTime() > 0ms ? 40ms : 1ms;
                        //packetLen = 40Mb;
                        @display("p=155,136;i=device/mainframe");
                }

                rosDataIMU: SensorClusterROSApplication {
                    parameters:
                        numSensors = numIMU;
                        packetRep = packetRepresentation;
                        packData = intuniform(0, 1);
                        @display("p=155,208;i=device/mainframe");
                }

                client: RosApplicationLayerHost {
                    parameters:
                        numApps = nos;
                        @display("p=357,136;i=device/mainframe");
                }

            connections:
                for i=0..numRadar-1, if !rosDataRadar.useSingleOut {
                    rosDataRadar.out++ --> client.tcpAppIn++;
                }

                for i=0..numIMU-1, if !rosDataIMU.useSingleOut {
                    rosDataIMU.out++ --> client.tcpAppIn++;
                }

                for i=0..numCamera-1, if !rosDataCamera.useSingleOut {
                    rosDataCamera.out++ --> client.tcpAppIn++;
                }
        }

        module PC2 extends PC
        {
            parameters:
                // Distribution of different sensors
                int numRadar = default(0);
                int numIMU = default(0);
                int numCamera = default(0);
                int nos = numRadar+numIMU+numCamera; // Number of sesnsors: This number will decide the number of TCP client and server apps

            gates:
                inout g;

            submodules:
                server: LVApplicationLayerHost {
                    parameters:
                        @networkNode;
                        numApps = nos;
                        @display("p=604,136;i=device/mainframe");
                }

                sinkCam: ClusterSink {
                    parameters:
                        @display("p=851,136;i=device/memory");
                        numSensorStreams = numCamera;
                }
                sinkRad: ClusterSink {
                    parameters:
                        @display("p=851,59;i=device/memory");
                        numSensorStreams = numRadar;
                }
                sinkIMU: ClusterSink {
                    parameters:
                        @display("p=850,240;i=device/memory");
                        numSensorStreams = numIMU;
                }

            connections:
                for i=0..numRadar-1{
                    server.tcpAppOut++ --> sinkRad.in++;
                }

                for i=0..numIMU-1 {
                    server.tcpAppOut++ --> sinkIMU.in++;
                }

                for i=0..numCamera-1 {
                    server.tcpAppOut++ --> sinkCam.in++;
                }
        }

    submodules:
        configurator: Ipv4NetworkConfigurator {
            parameters:
                @display("p=290,100;is=s");
        }

        queue: DropTailQueue {
            packetCapacity = 10;
            @display("p=357,100");
        }

        pc1: PC1 {
            @display("p=100,100");
        }

        pc2: PC2 {
            @display("p=200,100");
        }

    connections:
        pc1.g <--> C <--> pc2.g;
}

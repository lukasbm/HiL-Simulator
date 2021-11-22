package ad_hil_ethernet.simulations.MRGen;

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

network MRGen_CAN_stream
{
    parameters:
        string packetRepresentation @units("bitCount","bits","byteCount","bytes","applicationPacket") = default("bits");
        // Distribution of different sensors
        int numIMU = default(0);
        int nos = numIMU; // Number of sesnsors: This number will decide the number of TCP client and server apps

    types:
        channel C extends Eth1G
        {
            length = default(3m);
            ber = default(0);
            per = default(0);

        }

    submodules:
        rosDataIMU: SensorClusterROSApplication {
            parameters:
                numSensors = numIMU;
                packetRep = packetRepresentation;
                packData = intuniform(0, 1);
                //streamingDuration = 10s;
                //streamFileSize = 1150kB;//2300kB;
                //productionIntervl = simTime() > 0ms ? 10ms : 1ms;
                //packetLen = 130b;
                //useSingleOut = true;
                @display("p=166,136;i=device/mainframe");
        }

        client: RosApplicationLayerHost {
            parameters:
                numApps = nos;
                @display("p=357,136;i=device/mainframe");
        }

        server: LVApplicationLayerHost {
            parameters:
                numApps = nos;
                @display("p=604,136;i=device/mainframe");
        }

        configurator: Ipv4NetworkConfigurator {
            parameters:
                @display("p=300,200;is=s");
        }

        queue: DropTailQueue {
            packetCapacity = 10;
        }

        sinkIMU: ClusterLabVIEW {
            @display("p=775,136");
        }
    connections:
        for i=0..numIMU-1, if !rosDataIMU.useSingleOut {
            //rosDataIMU.out++  --> client.tcpAppIn[numRadar+numCamera+i];
            rosDataIMU.out++ --> client.tcpAppIn++;
            client.ethg++ <--> C <--> server.ethg++;
	        server.tcpAppOut++ --> sinkIMU.in++;
        }


}

network MRGen_CAN_stream_Tracedriven
{
    parameters:
        string packetRepresentation @units("bitCount","bits","byteCount","bytes","applicationPacket") = default("bits");
        // Distribution of different sensors
        int numIMU = default(0);
        int nos = numIMU; // Number of sesnsors: This number will decide the number of TCP client and server apps

    types:
        channel C extends Eth1G
        {
            length = default(3m);
            ber = default(0);
            per = default(0);
        }

    submodules:
        rosDataIMU: SensorClusterROSApplicationTracedriven {
            parameters:
                numSensors = numIMU;
                packetRep = packetRepresentation;
                packData = intuniform(0, 1);
                //streamingDuration = 10s;
                //streamFileSize = 1150kB;//2300kB;
                //productionIntervl = simTime() > 0ms ? 10ms : 1ms;
                //packetLen = 130b;
                //useSingleOut = true;
                @display("p=166,136;i=device/mainframe");
        }

        client: RosApplicationLayerHost {
            parameters:
                numApps = nos;
                @display("p=357,136;i=device/mainframe");
        }

        server: LVApplicationLayerHost {
            parameters:
                numApps = nos;
                @display("p=604,136;i=device/mainframe");
        }

        configurator: Ipv4NetworkConfigurator {
            parameters:
                @display("p=300,200;is=s");
        }

        queue: DropTailQueue {
            packetCapacity = 10;
        }

        sinkIMU: ClusterLabVIEWTracedriven {
            @display("p=775,136");
        }
    connections:
        for i=0..numIMU-1, if !rosDataIMU.useSingleOut {
            //rosDataIMU.out++  --> client.tcpAppIn[numRadar+numCamera+i];
            rosDataIMU.out++ --> client.tcpAppIn++;
            client.ethg++ <--> C <--> server.ethg++;
	        server.tcpAppOut++ --> sinkIMU.in++;
        }


}


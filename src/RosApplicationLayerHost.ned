package ad_hil_ethernet;
 
import inet.applications.contract.IApp;
import inet.common.MessageDispatcher;
import inet.node.base.TransportLayerNodeBase;
import inet.queueing.queue.PacketQueue;
import ad_hil_ethernet.RosTcpClientApp;
import ned.DelayChannel;

// The motivation for creating this Host module is to have a seperate host for the HiLPC side  
// for ease of customization.
// This module can be given data packets from external sources
module RosApplicationLayerHost extends CustTransportLayerNodeBase
{
    parameters:
        int numApps = default(1);
        bool addInterPackDelay = default(false);
        bool addPacketDropper = default(false);
        @figure[applicationLayer](type=rectangle; pos=250,6; size=1000,130; lineColor=#808080; cornerRadius=5; fillColor=#ffff00; fillOpacity=0.1);
        @figure[applicationLayer.title](type=text; pos=1245,11; anchor=ne; text="application layer");
        @display("i=device/pc2");
        @figure[submodules];
        forwarding = default(false);  // disable routing by default
        ipv4.arp.proxyArpInterfaces = default(""); // proxy arp is disabled on hosts by default
        *.routingTableModule = default("^.ipv4.routingTable");


    gates:
        input tcpAppIn[numApps] @loose; //For input to the TCP Apps
	   //inout tcpAppIO[numApps] @loose; //For input to the TCP Apps  

    types:
        //
        // To model delay of transmission of Camera data from Ros to TCP Client
        //
        channel DC extends DelayChannel
        {
            delay = truncnormal(1.45us,2.32us);// 40 ms transmission delay
        }

    submodules:
//        app[numApps]: RealTcpApp {
//            parameters:
//                @display("p=375,76,row,150");
//                ind = index;
//        }

        app[numApps]: RosTcpClientApp {
            parameters:
                @display("p=375,76,row,150");
                ind = index;
        }
        at: MessageDispatcher {
            parameters:
                @display("p=750,146;b=1000,5,,,,1");
        }

        // Adding a packet delayer
        packDelayer: CustPacketDelayer;

        packetDropper: PacketDropper {
            @display("p=998.55,239.4");
        }

        // Adding a passive queue to initialy store the packets
//        pckCollector[numApps]: PacketQueue{
//            @display("p=300,76,row,150");
//        } 

    connections allowunconnected:
        for i=0..numApps-1 {
            tcpAppIn[i] --> app[i].packetIn;
            //         tcpAppIn[i] --> pckCollector[i].in; // Adding passive queue for initial packet store
            //          pckCollector[i].out --> app[i].packetIn;
            app[i].socketOut --> at.in++;
            app[i].socketIn <-- at.out++;

        }

        at.out++ --> udp.appIn if hasUdp;
        at.in++ <-- udp.appOut if hasUdp;

        at.out++ --> tcp.appIn if hasTcp;
        at.in++ <-- tcp.appOut if hasTcp;
        if addInterPackDelay {
            tcp.ipOut --> packDelayer.in if hasTcp;
            packDelayer.out --> tn.in++ if hasTcp;
            //tcp.ipOut --> DC --> tn.in++ if hasTcp;
        }
        if addPacketDropper {
            tcp.ipOut --> packetDropper.in if hasTcp;
            packetDropper.out --> tn.in++ if hasTcp;
            //tcp.ipOut --> DC --> tn.in++ if hasTcp;
        }
        if (addInterPackDelay && addPacketDropper) {
            tcp.ipOut --> packetDropper.in if hasTcp;
            packetDropper.out --> packDelayer.in if hasTcp;
            packDelayer.out --> tn.in++ if hasTcp;
        }

        if !(addInterPackDelay||addPacketDropper) {
            tcp.ipOut --> tn.in++ if hasTcp;
        }

        tcp.ipIn <-- tn.out++ if hasTcp;

        at.out++ --> sctp.appIn if hasSctp;
        at.in++ <-- sctp.appOut if hasSctp;

        at.out++ --> tn.in++;
        at.in++ <-- tn.out++;

}

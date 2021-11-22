package ad_hil_ethernet;
 
import inet.applications.contract.IApp;
import inet.common.MessageDispatcher;
import inet.node.base.TransportLayerNodeBase;
import ned.DelayChannel;


module LVApplicationLayerHost2directions extends CustTransportLayerNodeBase//TransportLayerNodeBase
{
    parameters:
        int numApps = default(1);
        bool addInterPackDelay = default(false); // To support delay element
        @figure[applicationLayer](type=rectangle; pos=250,6; size=1000,130; lineColor=#808080; cornerRadius=5; fillColor=#ffff00; fillOpacity=0.1);
        @figure[applicationLayer.title](type=text; pos=1245,11; anchor=ne; text="application layer");
        @display("i=device/pc2");
        @figure[submodules];
        forwarding = default(false);  // disable routing by default
        ipv4.arp.proxyArpInterfaces = default(""); // proxy arp is disabled on hosts by default
        *.routingTableModule = default("^.ipv4.routingTable");

    gates:
        //For output to the TCP Apps
        // output tcpAppOut[numApps];
        output tcpAppOut[];

    types:
        //
        // To model delay of transmission of Camera data from Ros to TCP Client
        //
        channel DC extends DelayChannel
        {
            delay @unit(us) = truncnormal(33.26,14.75);// 40 ms transmission delay
        }

    submodules:
        app[numApps]: LVTcpServerApp {
            parameters:
                @display("p=375,76,row,150");
                ind = index;
        }
        at: MessageDispatcher {
            parameters:
                @display("p=750,146;b=1000,5,,,,1");
        }

         // Adding a packet delayer
        packDelayer: CustPacketDelayer {
           // delay = 2us;            
            @display("p=899.32495,225.22499");
        }

    connections allowunconnected:
        for i=0..numApps-1 {
            app[i].socketOut --> at.in++;
            app[i].socketIn <-- at.out++;
            // app[i].packetOut --> tcpAppOut[i];
            app[i].packetOut[0] --> tcpAppOut[i] if exists(app[i].packetOut);
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
        if !(addInterPackDelay) {
            tcp.ipOut --> tn.in++ if hasTcp;
        }
     //   if addInterPackDelay{            
     //       packDelayer.out --> tcp.ipIn if hasTcp;
    //        packDelayer.in <-- tn.out++ if hasTcp;
    //        //tcp.ipIn <-- DC <-- tn.out++ if hasTcp;
    //    }  

        tcp.ipIn <-- tn.out++ if hasTcp;

        at.out++ --> sctp.appIn if hasSctp;
        at.in++ <-- sctp.appOut if hasSctp;

        at.out++ --> tn.in++;
        at.in++ <-- tn.out++;


}

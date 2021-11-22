package ad_hil_ethernet;

import inet.applications.contract.IApp;
import inet.queueing.contract.IPassivePacketSink;
import inet.queueing.contract.IActivePacketSource;
import inet.applications.tcpapp.TcpServerSocketIo;
import inet.queueing.queue.PacketQueue;

module LVTcpServerConnection like IApp
{
    parameters:
        @display("i=block/cogwheel");
    gates:
        input socketIn;
        output socketOut;
        output packetOut;
    submodules:
//        source: <default("ActivePacketSource")> like IActivePacketSource {
//            parameters:
//                @display("p=100,100");
//        }
        LVBuff: PacketQueue {
            parameters:
                @display("p=200,100");
        }
        io: TcpServerSocketIo {
            parameters:
                @display("p=150,200");
        }
     connections allowunconnected:
      // source.out --> io.trafficIn;
       io.socketOut --> socketOut;
       socketIn --> io.socketIn;
       io.trafficOut --> packetOut;
       //io.trafficOut --> NIHiLBuff.in;
        
}

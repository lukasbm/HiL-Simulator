package ad_hil_ethernet;
 
import inet.applications.contract.IApp;
import inet.common.MessageDispatcher;


module LVTcpServerApp like IApp
{
    parameters:
        @display("i=block/app");
        volatile int basePort = default(1000); // For decising if port should start from 1000 or 2000 or anyother value
        volatile int ind = default(0); // for storing the index
        
    gates:
        input socketIn;
        output socketOut;
        output packetOut[];
        
    submodules:
//        listener: <default("LVTcpServerListener")> like IApp {
//            parameters:
//                @display("p=100,100");
//                localPort = basePort + ind;
//        }
        listener: LVTcpServerListener {
            parameters:
                @display("p=100,100");
                localPort = basePort + ind;
        }
        connection[0]: LVTcpServerConnection {
            parameters:
                @display("p=200,100,row,50");
        }
        ta: MessageDispatcher {
            parameters:
                @display("p=100,200");
        }
 
     connections:
        ta.out++ --> listener.socketIn;
        listener.socketOut --> ta.in++;
        ta.out++ --> { @display("m=s"); } --> socketOut;
        socketIn --> { @display("m=s"); } --> ta.in++;
        //connection[0].packetOut --> packetOut++ if exists(connection);
            
}

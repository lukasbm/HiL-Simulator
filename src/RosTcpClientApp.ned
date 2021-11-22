package ad_hil_ethernet;
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
//

import inet.applications.contract.IApp;
import inet.queueing.contract.IPassivePacketSink;
import inet.queueing.contract.IActivePacketSource;
import inet.applications.tcpapp.TcpClientSocketIo;
import inet.common.misc.ThruputMeter;

import ad_hil_ethernet.RosTcpClientSocketIo;

//
// This module is a generic TCP client application.
// The traffic sink modules can be built from queueing model elements.
// The packets can be given as inputs to the client using the packetIn gate
//
// @see ~TcpServerApp
//
module RosTcpClientApp like IApp
{
    parameters:
        @display("i=block/app");
        volatile int basePort = default(1000); // For decising if port should start from 1000 or 2000 or anyother value
        volatile int ind = default(0); // For storing the index

    gates:
        input socketIn;
        output socketOut;
        input packetIn; // For external packet input
        //inout packetIO;
    submodules:
        sink: <default("PassivePacketSink")> like IPassivePacketSink {
            parameters:
                @display("p=200,100");
        }
        io: RosTcpClientSocketIo {
            parameters:
                @display("p=150,200");
                connectPort = basePort + ind;
        }
    connections:
        packetIn --> io.trafficIn;
        //packetIO --> io.trafficIn;
        io.socketOut --> socketOut;
        socketIn --> io.socketIn;
        io.trafficOut --> sink.in;
}

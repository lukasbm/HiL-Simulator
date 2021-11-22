package ad_hil_ethernet;

import inet.applications.tcpapp.TcpClientSocketIo;

simple RosTcpClientSocketIo extends TcpClientSocketIo
{
    parameters:
        double startTime @unit(s) = default(10us); // time first session begins
        @class(inet::RosTcpClientSocketIo);
    
}

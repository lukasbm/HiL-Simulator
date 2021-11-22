package ad_hil_ethernet;

import inet.applications.tcpapp.TcpServerListener;

simple LVTcpServerListener extends TcpServerListener
{
    parameters:
        serverConnectionModuleType = default("ad_hil_ethernet.LVTcpServerConnection"); // setting connection moduletype to "LVTcpServerConnection"
        // For extending a simple module from another simple module,
        //to refer to a new class use the below tag
        @class(inet::LVTcpServerListener);
}


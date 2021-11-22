package ad_hil_ethernet;

//
// TODO auto-generated module
// This module can be configured to drop packets
//
simple PacketDropper
{
    parameters:
        volatile double dropTime @unit(s) = default(0s);
        volatile int packetNumber = default(0);
        @display("i=block/delay");
    gates:
        input in @labels(push);
        output out @labels(push);    
}

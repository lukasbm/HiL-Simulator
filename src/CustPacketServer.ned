package ad_hil_ethernet;

import inet.queueing.server.PacketServer;

import ad_hil_ethernet.scheduler.IScheduled;
//
// This module repeatedly pops packets from the connected packet provider and
// after a processing delay it pushes the packet into the connected packet
// consumer. The processing delay is processingTime + packetLength / processingBitrate.
//
simple CustPacketServer  extends PacketServer like IScheduled
{
    parameters:
        bool usingPCsubtype;
        int process = default(-1);
        volatile int priority;
        bool schedule = default(true);
        
        @class(CustPacketServer);
    
    gates:        
		input scheduler @direct;
}

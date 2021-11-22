package ad_hil_ethernet;

import inet.queueing.base.PacketSourceBase;
import inet.queueing.contract.IActivePacketSource;

import ad_hil_ethernet.scheduler.IScheduled;
//
// This module is an active packet source which pushes packets into the connected
// packet consumer.
//
simple CustActivePacketSourceTracedriven extends PacketSourceBase like IActivePacketSource, IScheduled
{
    parameters:
        bool usingPCsubtype;
        int process = default(-1);
        volatile int priority;
        bool schedule = default(true);

        string productionInterval @unit(s); // elapsed time between subsequent packets pushed to the connected packet consumer
        volatile double processingTime @unit(s); // processing time required to spawn a packet.
        
        @class(CustActivePacketSourceTracedriven);
        @display("i=block/source");

        @signal[packetCreated](type=inet::Packet);
        @statistic[packetCreated](title="packets created"; record=count,sum(packetBytes),vector(packetBytes); interpolationmode=none);
        
        @signal[packetID](type=long);
        @statistic[packetID](title="packets id"; record=vector);
        
    gates:
        output out @labels(push);
        input scheduler @direct;
}

package ad_hil_ethernet;

import inet.queueing.server.PacketServer;

import ad_hil_ethernet.scheduler.IScheduled;
//
// This module repeatedly pops packets from the connected packet provider and
// after a processing delay it pushes the packet into the connected packet
// consumer. The processing delay is processingTime + packetLength / processingBitrate.
//
simple CustPacketServerWithBufferTimeAndTimeStamp  extends PacketServer like IScheduled
{
    parameters:
        bool usingPCsubtype;
        int process = default(-1);
        volatile int priority;
        bool schedule = default(true);
        
        volatile double bufferInterval @unit(s); // elapsed time for pre buffering
        @class(CustPacketServerWithBufferTimeAndTimeStamp);
        @signal[endToEndDelay](type=simtime_t);
        @statistic[endToEndDelay](title="End to End Delay of a packet"; record=vector);
        @signal[packetID](type=long);
        @statistic[packetID](title="packet ID"; record=vector);
        @signal[schedulingDiff](type=simtime_t);
        @statistic[schedulingDiff](title="Difference between scheduling and simTime"; record=vector);
        @signal[creatingDiff](type=simtime_t);
        @statistic[creatingDiff](title="Difference between creating Time of the (same) packet"; record=vector);
        
    gates:        
		input scheduler @direct;
}

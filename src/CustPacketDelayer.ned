package ad_hil_ethernet;

import inet.queueing.base.PacketProcessorBase;
import inet.queueing.contract.IActivePacketSource;
import inet.queueing.contract.IPassivePacketSink;

import ad_hil_ethernet.scheduler.IScheduled;
//
// This module connects one packet producer to one packet consumer. It can be
// pushed with packets from the connected packet producer. When this happens,
// the packet is delayed and eventually pushed to the output. Note that the
// order of packets may change if the delay parameter is configured to be a
// distribution.
//
simple CustPacketDelayer extends PacketProcessorBase like IPassivePacketSink, IActivePacketSource, IScheduled
{
    parameters:
        bool usingPCsubtype;
        int process = default(-1);
        volatile int priority;
        bool schedule = default(true);
        
        volatile double delay @unit(s);
        @class(CustPacketDelayer);
        @display("i=block/delay");
    gates:
        input in @labels(push);
        output out @labels(push);
        input scheduler @direct;
}
package ad_hil_ethernet;

simple PacketProcessor
{
    parameters:
        volatile int packetLength @unit(b); // maximum total length of packets
        string displayStringTextFormat = default("contains %p pushed %u\ncreated %o popped %r");
        @class(PacketProcessor);
        @signal[packetPushed](type=inet::Packet);
        @signal[packetMergedSent](type=inet::Packet);
        @statistic[packetPushed](title="packet pushed"; record=count,sum(packetBytes),vector(packetBytes); interpolationmode=none);
        @statistic[packetMergedSent](title="packet Merged and Sent"; record=count,sum(packetBytes),vector(packetBytes); interpolationmode=none);
        @statistic[queueingTime](title="queueing times"; source=queueingTime(packetMergedSent); record=histogram,vector; unit=s; interpolationmode=none);
        //@statistic[queueLength](title="queue length"; source=count(packetPushed) - count(packetPopped) - count(packetRemoved) - count(packetDropped); record=max,timeavg,vector; interpolationmode=sample-hold);
    
    gates:
        input in @loose;
        output out @loose;    
}

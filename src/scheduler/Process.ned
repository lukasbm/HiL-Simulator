package ad_hil_ethernet.scheduler;

//
// Simple (empty) Module representing a Process (for clustering)
// Note: Make sure you have "connections allowunconnected" in the network you intend to place the scheduler.
simple Process
{
    gates:
        input in @direct; // from modules to this process
        input scheduler @direct; // from scheduler to this process
}

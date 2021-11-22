package ad_hil_ethernet.scheduler;

// Simple Module representing the Scheduler
// Note: Make sure you have "connections allowunconnected" in the network you intend to place the scheduler.
moduleinterface Scheduler
{   
    parameters:
        int cores;
        int frequency @unit(MHz);
        int measurement_frequency @unit(MHz);
        int min_priority;
        int max_priority; 
    gates:
        input in @direct;
}

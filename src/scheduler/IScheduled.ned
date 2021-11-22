package ad_hil_ethernet.scheduler;

moduleinterface IScheduled
{
    parameters:
        bool usingPCsubtype;
        int process;
        volatile int priority;
        bool schedule; 
    gates:        
		input scheduler @direct;
}

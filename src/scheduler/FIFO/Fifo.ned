package ad_hil_ethernet.scheduler.FIFO;

import ad_hil_ethernet.scheduler.Scheduler;

simple Fifo like Scheduler  {
    parameters:
    	int cores;
    	int frequency @unit(MHz) = default(1000MHz);
   		int measurement_frequency @unit(MHz) = default(1000MHz);
   		int min_priority = default(1);
        int max_priority = default(99);
    gates:
        input in @direct;
}

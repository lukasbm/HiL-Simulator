package ad_hil_ethernet.scheduler.RR;

import ad_hil_ethernet.scheduler.Scheduler;

simple RoundRobin like Scheduler  {
    parameters:
    	int cores;
    	int frequency @unit(MHz) = default(1000MHz);
   		int measurement_frequency @unit(MHz) = default(1000MHz);
		double quantum @unit(s) = default(0.5ms);
   		int min_priority = default(1);
        int max_priority = default(99);
    gates:
        input in @direct;
}

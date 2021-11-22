package ad_hil_ethernet;

import ad_hil_ethernet.scheduler.Scheduler;
import ad_hil_ethernet.scheduler.CPUCore;
import ad_hil_ethernet.scheduler.Process;

module PC
{
    parameters:
        int cores;
        string strategy;
        int numProcesses = default(0);

    submodules:
        processes[numProcesses]: Process {
            @display("p=104,35,r");
        }

        scheduler: <strategy> like Scheduler {
            cores = cores;
            @display("p=104,100");
        }

        cpuCores[cores]: CPUCore {
            @display("p=104,160,r");
        }

    connections allowunconnected:
}

import matplotlib.pyplot as plt
from typing import Dict
import json

data: Dict
with open("../simulations/MRGen_for_validation_PC/results/output.json") as f:
    data = dict(json.load(f))

for pc in data:
    for core in data[pc]["cpuUsage"]:
        values = [p["value"] for p in data[pc]["cpuUsage"][core]]
        times = [1000 * p["time"] for p in data[pc]["cpuUsage"][core]]

        x = []
        y = []
        for (t, v) in zip(times, values):
            # if v != 0:
            x.append(t)
            y.append(v)


        plt.plot(x, y, label=f"{pc} - c{core}")

plt.xlabel("time in milliseconds")
plt.ylabel("utilisation")
plt.title("utilization per core by time")
plt.legend(loc="upper right")
# plt.savefig("output/cpu-usage-plot.pdf")
plt.show()

from typing import Dict
import matplotlib.pyplot as plt
import json
from typing import Dict

data: Dict
with open("../simulations/MRGen_for_validation_PC/results/output.json") as f:
    data = dict(json.load(f))

cores = []
values = []

for pc in data:
    for core in data[pc]["cpuUsage"]:
        usages = [x["value"] for x in data[pc]["cpuUsage"][core] ]
        cores.append(f"{pc} - c{core}")
        values.append(sum(usages) / len(usages))

plt.bar(cores, values)
plt.xlabel("core")
plt.ylabel("utilisation")
plt.title("utilization over the entire simulation time per core")
# plt.savefig("output/cpu-usage-total.pdf")
plt.show()

import matplotlib.pyplot as plt
from typing import Dict
import json

data: Dict
with open("../simulations/Test_for_verification/results/output.json") as f:
    data = dict(json.load(f))

PC = "pc1"

def chunks(list_in, n):  # helper
    for i in range(0, len(list_in), n):
        yield list_in[i:i+n]


usage1 = list(map(lambda x: x["value"] , data[PC]["cpuUsage"]["0"]))
usage1_100ms = [sum(c)/len(c) for c in chunks(usage1, 100)]
usage1_10ms = [sum(c)/len(c) for c in chunks(usage1, 10)]

usage2 = list(map(lambda x: x["value"] , data[PC]["cpuUsage"]["1"]))
usage2_100ms = [sum(c)/len(c) for c in chunks(usage2, 100)]
usage2_10ms = [sum(c)/len(c) for c in chunks(usage2, 10)]


fig, (ax1, ax2) = plt.subplots(1,2)
plt.subplots_adjust(bottom=0.15)

#CORE 1
ax1.set_title("Core 1")
ax1.plot([t for t in range(0, len(usage1), 100)], usage1_100ms, label=f"100ms interval")
ax1.plot([t for t in range(0, len(usage1), 10)], usage1_10ms, label=f"10ms interval")
# ax1.plot([t for t in range(0, len(usage1), 1)], usage1, label=f"1ms")

#CORE 2
ax2.set_title("Core 2")
ax2.plot([t for t in range(0, len(usage2), 100)], usage2_100ms, label=f"100ms interval")
ax2.plot([t for t in range(0, len(usage2), 10)], usage2_10ms, label=f"10ms interval")
# ax2.plot([t for t in range(0, len(usage2), 1)], usage2, label=f"1ms")

ax1.set_xlabel("time in milliseconds")
ax2.set_xlabel("time in milliseconds")

ax1.set_ylabel("utilisation")
ax2.set_ylabel("utilisation")

ax1.legend(loc="upper right")
ax2.legend(loc="upper right")

plt.show()

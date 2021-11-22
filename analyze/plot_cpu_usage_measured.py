import csv
import matplotlib.pyplot as plt
from datetime import datetime

#
# plottet die gemessenen cpu auslastungen diverser prozesse vom HiL system
#

res = {}

with open("input/third_measure/kscc26398_nodup.csv") as file:
    reader = csv.DictReader(file, delimiter=";")
    for row in reader:
        t = datetime.fromisoformat(row["Timestamp"])
        u = float(row["CPU_Percentage"])
        if t not in res:
            res[t] = {
                "replayer_n+": 0.0,
                "Adapter_to+": 0.0,
                "com_tcp_cl+": 0.0,
            }

        if row["Process"] == "replayer_n+" and u > res[t]["replayer_n+"]:
            res[t]["replayer_n+"] = u
        if row["Process"] == "Adapter_to+" and u > res[t]["Adapter_to+"]:
            res[t]["Adapter_to+"] = u
        if row["Process"] == "com_tcp_cl+" and u > res[t]["com_tcp_cl+"]:
            res[t]["com_tcp_cl+"] = u


# calc total
for t in res:
    res[t]["total"] = sum(res[t].values())

# sort dict
res = dict(sorted(res.items()))

# processes
# plt.plot(res.keys(), list(map(lambda x: x["replayer_n+"], res.values())), label="replayer_n+")
# plt.plot(res.keys(), list(map(lambda x: x["Adapter_to+"], res.values())), label="Adapter_to+")
# plt.plot(res.keys(), list(map(lambda x: x["com_tcp_cl+"], res.values())), label="com_tcp_cl+")
# res
plt.plot(res.keys(), list(map(lambda x: x["total"], res.values())), label="total")

plt.legend()
plt.show()

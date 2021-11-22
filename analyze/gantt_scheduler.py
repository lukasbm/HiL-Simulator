import matplotlib.pyplot as plt
import matplotlib.ticker as ticker
from typing import Dict
import json

data: Dict
with open("../simulations/Test_for_verification/results/output.json") as f:
    data = dict(json.load(f))

#
# plottet simulierte zeiten des schedule modules als gantt chart (output.json)
#

MAX_TASKS = 15
PC = "pc1"
# MAX_TIME = 0
# for tid in data[PC]["tasks"]:
#     t = data[PC]["tasks"][tid]
#     if "completed" in t and t["completed"] > MAX_TIME:
#         MAX_TIME = t["completed"]


def get_color(c):
    if c == 0:
        color = "red"
    elif c == 1:
        color = "blue"
    elif c == 2:
        color = "green"
    elif c == 3:
        color = "orange"
    elif c == 4:
        color = "purple"
    elif c == 5:
        color = "yellow"
    elif c == 6:
        color = "cyan"
    else:
        color = "black"

    return color


# plot bars
fig, ax = plt.subplots(figsize=(20, 10))

for idx in list(sorted([int(d) for d in data[PC]["tasks"]]))[:MAX_TASKS]:
    t = data[PC]["tasks"][str(idx)]
    if "release" not in t or "completed" not in t:
        continue

    if t["run"][0] == t["completed"]:  # damit nur tasks mit tatsächlicher ausführungszeit angezeigt werden
        continue

    for i in range(len(t["release"])):
        col = get_color(t["release"][i]["core"])
        start = t["assign"][i]["time"]
        end = t["release"][i]["time"]
        # if end - start <= 0:
        #     end += 0.0001

        e_left = (end - t["add"]) * 1000
        e_right = (t["completed"] - end) * 1000
        # plot
        ax.barh(
            y=t["name"] + " " + str(idx),
            left=start*1000,
            width=1000*(end - start),
            color=col,
            height=0.5,
            xerr=[[e_left], [e_right]]
        )

# customize settings
plt.gca().invert_yaxis()
plt.title("Gantt chart of Task execution")
ax.set_xlabel('Milliseconds since start')
ax.set_ylabel('Task')
# ax.xaxis.set_major_locator(ticker.MultipleLocator(1))
plt.grid(axis='x', alpha=0.5)
plt.legend(loc="upper right")
# plt.savefig("output/scheduler-gantt.pdf")
plt.show()

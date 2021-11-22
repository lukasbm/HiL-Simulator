from reverse_mapping import *
from read_measured import results as mr
import numpy as np

with open("./traces.txt", "w") as file:
    for i in range(1,10):
        if f"T{i}" not in mr or f"dT{i}" not in mr or f"B{i}" not in mr:
            continue

        mapp = mapping[readin[(f"T{i}",f"dT{i}",f"B{i}")]]

        diff = [99999999999999] + mr[f"dT{i}"][:-1]
        delay = np.array(mr[f"T{i}"]) - np.array(mr[f"T{i-1}"])

        result = [min(x,y) for (x,y) in zip(diff, list(delay))]


        file.write(f"{str(mapp)}: \"")
        for r in result:
            file.write(f"{str(r)}s ")
        file.write("\"\r\n")

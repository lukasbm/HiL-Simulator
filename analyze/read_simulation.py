import csv
from mapping import *

#
# Wendet mapping auf die von OMNeT exporierte csv datei an und plottet die ergebnisse (diff times)
#
results = {}
def insert_result(key, value):
    if key not in results:
        results[key] = []

    results[key].append(value)

with open("../simulations/MRGen_for_validation_PC/results/output.csv") as file:
    reader = csv.reader(file, delimiter=",")
    # get and transform headers
    headers = list(next(reader))[::2]

    def hash_header(h: str):
        name = h.split(":")[0]
        module = h[h.rfind(".")+1:h.find("[")]
        return (module, name)

    def valid_header(h: str):
        res = False
        try:
            col_map = mapping[readin[h[1]]]
            res = True
        finally:
            return res

    headers = list(map(hash_header, headers))
    valid_headers = list(filter(valid_header, list(zip(range(0, 2*len(headers), 2), headers)) ))

    # read data and store them in results
    prev = None
    for row in reader:
        for (k, h) in valid_headers:
            time = row[k]
            val = row[k+1]
            last_time = prev[k] if prev else "0"
            if time == '' or val == '' or last_time == '':
                continue

            col_map = mapping[readin[h]]
            insert_result(col_map[0], float(time))  # T
            insert_result(col_map[1], float(time) - float(last_time))  # dT
            insert_result(col_map[2], float(val))  # B
        prev = row

# latencies
for i in range(1, 10):
    if f"T{i-1}" not in results or f"T{i}" not in results:
        continue
    results[f"T{i}-T{i-1}"] = [y - x for (x, y) in zip(results[f"T{i-1}"], results[f"T{i}"])]


clean_results = {}
for key in results:
    if key[0] == "d":
        l = list(filter(lambda x: x != 0, results[key]))
        if len(l) > 1: clean_results[key] = l
    elif key[0] == "T":
        clean_results[key] = results[key]

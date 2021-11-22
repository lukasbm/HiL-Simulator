import pandas

#
# liest gegebene daten in gewünschtem format ein
# daten kommen aus CAN.csv und beinhalten T*, dT* und B* informationen/timings
#

# dT spalten: diff in nanosekunden (timedelta)
# T spalten: timestamp in epochtime, nanosekunden
# B spalten: packet size in Byte

results = {}
md = pandas.read_csv("input/second_measure/CAN.csv", delimiter=";")

for i in range(0, 10):
    try:
        l = list(md[f"T{i}"])
        results[f"T{i}"] = list(map(lambda x: (x - l[0]) / 1000000000, l))
    except KeyError:
        pass

    try:
        l = list(md[f"T{i}"])
        results[f"T{i}"] = list(map(lambda x: (x - l[0]) / 1000000000, l))
    except KeyError:
        pass

    try:
        results[f"dT{i}"] = list(map(lambda x: x / 1000000000, md[f"dT{i}"]))
    except KeyError:
        pass

    try:
        results[f"dT{i}"] = list(map(lambda x: x / 1000000000, md[f"dt{i}"]))
    except KeyError:
        pass

    try:
        results[f"B{i}"] = list(md[f"b{i}"])
    except KeyError:
        pass

    try:
        results[f"B{i}"] = list(md[f"B{i}"])
    except KeyError:
        pass

    # calculate dT if not present in csv file
    if f"T{i}" in results and f"dT{i}" not in results:
        results[f"dT{i}"] = []
        prev = results[f"T{i}"][0]
        for ts in results[f"T{i}"]:
            results[f"dT{i}"].append(ts - prev)
            prev = ts

# latencies
for i in range(1, 10):
    try:
        results[f"T{i}-T{i-1}"] = list(map(lambda x: x / 1000000000, md[f"T{i}-T{i-1}"]))
    except:
        print(f"latency between T{i} and T{i-1} not found in CSV")
        # FIXME div by 1000000000??
        results[f"T{i}-T{i-1}"] = [y - x for (x, y) in zip(results[f"T{i-1}"], results[f"T{i}"])]


clean_results = {}
for key in results:
    if key[0] == "d":
        avg = sum(results[key]) / len(results[key])
        min_d = min(results[key])
        max_d = max(results[key])
        clean_results[key] = list(filter(lambda x: x > min_d+(avg/3) and x < max_d-(avg/3) , results[key]))
    elif key[0] == "T":
        clean_results[key] = results[key]

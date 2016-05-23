import inspect
import subprocess
import json



def test(file, traversal = "rec", Ki = "20", Kt = "15", build = "dfs", complexity = "nlogn", heuristic = "sah", method = "kdtree", size = "1", output = "none", time = "-1"):
    command = "D://TIPE//source//tipe//tipe//bin//Debug//tipe.exe " + file
    for name in inspect.getargspec(test)[0]:
        if name != "file":
            command += " --" + name + " " + eval(name)
    out = subprocess.check_output(command).decode("utf-8") 
    values = {o.strip().split(":")[0]: o.strip().split(":")[1].strip() for o in out.split('\n') if o}
    if not "build" in values:
        values["build"] = "INFTY"
    if not "box/ray" in values:
        values["box/ray"] = "0"
    if not "task" in values:
        values["task"] = "INFTY"
    if not "triangles" in values:
        values["triangles"] = "0"
    if not "tri/ray" in values:
        values["tri/ray"] = "0"
    return values
    

results = {"n2":{}, "nlog2n": {}, "nlogn": {}}
nbs = ["05", "1", "2", "3", "5", "10", "15", "30", "50", "65", "80", "90", "100"]
files = ["suzanne.json"]

cont = {"n2": True, "nlog2n": True, "nlogn": True}
TIME_LIMIT = 10*60
for n in nbs:
    f = "D://TIPE//source//tipe//tipe//bin//Debug//tests//boudha//boudha_"+n+".json"
    print(f)
    for c in ["n2", "nlogn", "nlog2n"]:
        if cont[c]:
            results[c][n] = test(f, complexity = c, time = str(TIME_LIMIT))
            print(results[c][n])
            if results[c][n]["build"] == "INFTY":
                cont[c] = False
        else:
            print("pass", c)
    #results [f] = test("D://TIPE//source//tipe//tipe//bin//Debug//tests//" + f, complexity = "n2")

with open('D://TIPE//source//tipe//tipe//bin//Debug//results_boudha.json', 'w') as outfile:
    json.dump(results, outfile, indent = 4)
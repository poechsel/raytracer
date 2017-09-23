import json
import os.path
#une scéne est simplement décrite par un fichier json, le code pour le parser
#est donc relativement court en python
def load(path):
    with open(path) as file:
        temp = json.load(file)
        if not "OBJECTS" in temp :
            print("OBJECTS not defined in scene file. Closing")
            return {}
        if not os.path.isabs(temp["OBJECTS"]['path']) :
            temp["OBJECTS"]["path"] = os.path.join(os.path.dirname(path), temp["OBJECTS"]["path"])
        return temp

    return {}


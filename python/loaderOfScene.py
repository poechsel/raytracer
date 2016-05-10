import json
def load(path):
    with open(path) as file:    
        return json.load(file)
    return {}


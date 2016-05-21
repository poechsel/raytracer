import json
#une scéne est simplement décrite par un fichier json, le code pour le parser
#est donc relativement court en python
def load(path):
    with open(path) as file:
        return json.load(file)
    return {}


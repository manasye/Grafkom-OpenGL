import json


def getIndexFromLabel(label, points):
    for point in points:
        if (point['label'] == label):
            return point['index']


with open('../data/unicorn.json') as f:
    data = json.load(f)

elements = data['geogebra']['construction']['element']
commands = data['geogebra']['construction']['command']

pointsTxt = []
points = []
polygonsTxt = []

idx = 0
for element in elements:
    if (element['-type'] == 'point'):
        points.append({'index': idx, 'label': element['-label']})
        pointsTxt.append('V({},{})'.format(
            float(element['coords']['-x']) / 15, float(element['coords']['-y']) / 15))
        idx += 1

for command in commands:
    if (command['-name'] == 'Polygon'):
        i1 = getIndexFromLabel(command['input']['-a0'], points)
        i2 = getIndexFromLabel(command['input']['-a1'], points)
        i3 = getIndexFromLabel(command['input']['-a2'], points)
        polygonsTxt.append('P({},{},{})'.format(i1, i2, i3))


with open('../data/unicorn.txt', 'w') as f:
    f.write("# Points\n")
    for point in pointsTxt:
        f.write("%s\n" % point)
    f.write("# Polygon\n")
    for polygon in polygonsTxt:
        f.write("%s\n" % polygon)

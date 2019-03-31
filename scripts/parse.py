import json

with open('../data/unicorn.json') as f:
    data = json.load(f)

elements = data['geogebra']['construction']['element']

pointsTxt = []
points = []
polygons = []

for element in elements:
    if (element['-type'] == 'point'):
        points.append({'label': element['-label']})
        pointsTxt.append('({},{})'.format(
            float(element['coords']['-x']) / 15, float(element['coords']['-y']) / 15))
    elif (element['-type'] == 'polygon'):
        polygons.append(element)

print(len(polygons))

with open('../data/unicorn.txt', 'w') as f:
    f.write("Points\n")
    for point in pointsTxt:
        f.write("%s\n" % point)
    f.write("Polygon\n")

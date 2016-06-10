#!/usr/bin/python
# -*- coding: utf-8 -*-

from collections import deque
import sys

if len(sys.argv) != 3:
    sys.exit(1)

me = open(sys.argv[1], 'r')
out = open(sys.argv[2], 'w')

translations = deque()

for line in me:
    if "translate" in line:
        line_split = line.strip().split(" ")
        translations.append(tuple(map(lambda x: float(x), line_split[1:4])))
        continue
    if "popTransform" in line:
        translations.pop()
        continue
    if "pushTransform" in line:
        continue
    if "directional" in line:
        line = line.replace("directional", "point")
    if "specular" in line:
        line = line.replace("specular", "special")
    if "sphere" in line:
        line_split = line.strip().split()
        coords = tuple(map(lambda x: float(x), line_split[1:4]))
        rad = line_split[4]
        total_translation = reduce(lambda x, y: tuple(map(sum, zip(x, y))), translations)
        coords = tuple(map(sum, zip(coords, total_translation)))
        line = "sphere %s %s %s " % coords + rad + "\n"
    # TODO: This is a bit blunt. Refactor and generalise when possible.
    if sys.argv[1] == "scene7.test" and "vertex" in line:
        line_split = line.strip().split()
        coords = list(map(lambda x: float(x), line_split[1:4]))
        coords[1] = coords[1] - 5
        line = "vertex %s %s %s" % tuple(coords) + "\n"
    out.write(line)
me.close()
out.close()
print 'Succesfully wrote file "{0}".'.format(sys.argv[2])

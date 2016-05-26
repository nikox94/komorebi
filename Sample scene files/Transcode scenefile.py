#!/usr/bin/python
# -*- coding: utf-8 -*-

from collections import deque

me = open("scene5.test", 'r')
out = open("scene5.test.out", 'w')

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
        line_split = line.strip().split(" ")
        coords = tuple(map(lambda x: float(x), line_split[1:4]))
        rad = line_split[4]
        total_translation = reduce(lambda x, y: tuple(map(sum, zip(x, y))), translations)
        coords = tuple(map(sum, zip(coords, total_translation)))
        line = "sphere %s %s %s " % coords + rad + "\n"
        print line
    out.write(line)
me.close()
out.close()

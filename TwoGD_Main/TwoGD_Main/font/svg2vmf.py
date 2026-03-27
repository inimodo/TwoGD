import re
import math
import os
from os import listdir
from os.path import isfile, join


CURVE_STEPS = 2



def parse_path(d):
    tokens = re.findall(r'[MLQZmlqz]|-?\d*\.?\d+', d)
    i = 0
    commands = []

    while i < len(tokens):
        cmd = tokens[i]
        i += 1

        if cmd.upper() == 'M':
            x = float(tokens[i]); y = float(tokens[i+1])
            i += 2
            commands.append(('M', x, y))

        elif cmd.upper() == 'L':
            x = float(tokens[i]); y = float(tokens[i+1])
            i += 2
            commands.append(('L', x, y))

        elif cmd.upper() == 'Q':
            x1 = float(tokens[i]); y1 = float(tokens[i+1])
            x2 = float(tokens[i+2]); y2 = float(tokens[i+3])
            i += 4
            commands.append(('Q', x1, y1, x2, y2))

        elif cmd.upper() == 'Z':
            commands.append(('Z',))

    return commands


def approx_quadratic(p0, p1, p2, steps=CURVE_STEPS):
    points = []
    for i in range(1, steps + 1):
        t = i / steps
        x = (1 - t)**2 * p0[0] + 2 * (1 - t) * t * p1[0] + t**2 * p2[0]
        y = (1 - t)**2 * p0[1] + 2 * (1 - t) * t * p1[1] + t**2 * p2[1]
        points.append((x, y))
    return points


def svg_to_custom(svg_path):
    with open("svg/"+svg_path+".svg", 'r') as f:
        content = f.read()

    path_data = re.findall(r'd="([^"]+)"', content)

    points = []
    lines = []

    for d in path_data:
        commands = parse_path(d)

        start_index = None
        prev = None

        for cmd in commands:
            if cmd[0] == 'M':
                prev = (cmd[1], cmd[2])
                start_index = len(points)
                points.append(prev)

            elif cmd[0] == 'L':
                new_pt = (cmd[1], cmd[2])
                points.append(new_pt)
                lines.append((len(points)-2, len(points)-1))
                prev = new_pt

            elif cmd[0] == 'Q':
                p0 = prev
                p1 = (cmd[1], cmd[2])
                p2 = (cmd[3], cmd[4])

                approx = approx_quadratic(p0, p1, p2)
                for pt in approx:
                    points.append(pt)
                    lines.append((len(points)-2, len(points)-1))
                prev = approx[-1]

            elif cmd[0] == 'Z':
                if start_index is not None:
                    points.append(points[start_index])
                    lines.append((len(points)-2, len(points)-1))

    rounded_points = [(round(x), round(y)) for x, y in points]

    filedata = f"v {len(rounded_points)} 1 {len(lines)}\n"

    for x, y in rounded_points:
        filedata = filedata+f"p {x} {y}\n"

    filedata = filedata+"c 255 255 255\n"

    for i, j in lines:
        filedata = filedata+f"l {i} {j} 0\n"
        
    with open("vmf/"+svg_path+".vmf", "w") as f:
      f.write(filedata)

if __name__ == "__main__":
    onlyfiles = [f for f in listdir("svg")]
    for file in onlyfiles:
        svg_to_custom(file.replace('.svg',""))


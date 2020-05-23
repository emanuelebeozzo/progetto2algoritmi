from PIL import Image, ImageDraw
import sys
import os

tileSize = 50
line_width = 2
padding = 10
innerPadding = 10

def getMap(filename):
    with open(filename, "r") as f:
        rows = f.readlines()

    n, m, b, w = map(int, rows[0].split())

    blacks = []
    whites = []

    for i in range(b):
        x, y = map(int, rows[1+i].split())
        blacks.append((x, y))

    for i in range(w):
        x, y = map(int, rows[1+b+i].split())
        whites.append((x, y))

    return n, m, blacks, whites

def getImage(map):

    n = map[0]
    m = map[1]
    blacks = map[2]
    whites = map[3]

    image = Image.new("L", (m*tileSize + padding*2, n*tileSize + padding*2), 255)
    draw = ImageDraw.Draw(image)

    #draw the matrix
    for i in range(n+1):
        draw.line([padding, padding + tileSize*i, padding + tileSize*m, padding + tileSize*i], fill=0, width=line_width)
    for i in range(m+1):
        draw.line([padding + tileSize*i, padding, padding + tileSize*i, padding + tileSize*n], fill=0, width=line_width)

    for point in blacks:
        drawDot(draw, point, tileSize - innerPadding*2)

    for point in whites:
        drawDot(draw, point, tileSize - innerPadding*2, False)

    return image

def getRoutes(filename):
    with open(filename, "r") as f:
        content = f.read().split("\n");

    routes = []

    for line in content:
        if(len(line) > 0 and line[-1] == "#"):
            line = line.split()
            start = (int(line[2]), int(line[3]))
            route = line[4][:-1]
            routes.append((start, route))

    return routes

def getCoordinates(point):
    return (padding + tileSize*point[1] + tileSize/2, padding + tileSize*point[0] + tileSize/2)

def makeMove(point, move):
    if(move == "U"):
        return (point[0]-1, point[1])
    elif(move == "D"):
        return (point[0]+1, point[1])
    elif(move == "L"):
        return (point[0], point[1]-1)
    elif(move == "R"):
        return (point[0], point[1]+1)
    else:
        raise Exception;

def drawDot(draw, point, size, fill=True):
    c = getCoordinates(point)
    start = (c[0] - size/2, c[1] - size/2)
    end = (c[0] + size/2, c[1] + size/2)
    draw.ellipse([start, end], fill=0 if fill else 255, outline=0, width=line_width)

def drawRoute(image, route):
    draw = ImageDraw.Draw(image)
    moves = route[1]
    point1 = route[0]
    dotSize = 6
    drawDot(draw, point1, dotSize)
    for move in moves:
        point2 = makeMove(point1, move)
        draw.line([getCoordinates(point1), getCoordinates(point2)], fill=0, width=line_width)
        point1 = point2
    drawDot(draw, point1, dotSize)
    return image

if __name__ == '__main__':
    map = getMap(sys.argv[1])
    image = getImage(map)
    routes = getRoutes(sys.argv[2])

    if not os.path.exists("output"):
        os.makedirs("output")

    for i, route in enumerate(routes):
        drawRoute(image.copy(), route).save(f"output/out_{i}.jpg")

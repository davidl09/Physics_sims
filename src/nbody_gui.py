#only works on linux. also macos if nbody.c is recompiled: gcc -fPIC -shared -o lib/nbody.so src/nbody.c

from ctypes import *
import pygame
import math
import pathlib
import random

MAX_COUNT = int(10)
WIDTH = 1000
HEIGHT = 1000
DT = 1/120
G = 10
WHITE = (255, 255, 255)
GREY = (30, 30, 30)
ROTATION = 1

class Vector(Structure):
    _fields_ = ("x", c_double), ("y", c_double)

class Body(Structure):
    _fields_ = ("pos", Vector), ("vel", Vector), ("accel", Vector), ("force", Vector), ("mass", c_double), ("dens", c_double)

path = pathlib.Path().absolute()
path = path / "lib/nbody.so"
lib = cdll.LoadLibrary(path)

fill_arr = lib.fill_arr
fill_arr.argtypes = [POINTER(Body), c_double, c_uint, c_uint, c_uint]
fill_arr.restype = None

calculate_step = lib.calculate_step
calculate_step.argtypes = [POINTER(Body), c_double, c_uint, c_double]
calculate_step.restype = None

array = (Body * MAX_COUNT)()

fill_arr(array, c_double(ROTATION/50), 1000, 1, c_uint(MAX_COUNT))

pygame.init()

screen = pygame.display.set_mode((WIDTH, HEIGHT))

clock = pygame.time.Clock()

while True:
    for event in pygame.event.get():
            if event.type == pygame.QUIT:
                pygame.quit()

    calculate_step(array, c_double(DT), c_uint(MAX_COUNT), c_double(G))

    screen.fill(WHITE)

    for body in array:
        pygame.draw.circle(screen, GREY, (body.pos.x + WIDTH/2, -body.pos.y + WIDTH/2), math.pow(body.mass/body.dens, 1/3))

    pygame.display.update()

    clock.tick(1/DT)
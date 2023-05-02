import pygame
import numpy as np
from ctypes import *

pygame.init()

file = open('sim_data.txt')
SCREEN_WIDTH = 

screen = pygame.display.set_mode((SCREEN_WIDTH, SCREEN_HEIGHT))

clock = pygame.time.Clock()

while True:
    for event in pygame.event.get():
            if event.type == pygame.QUIT:
                pygame.quit()
    
    obj = file.readline()
    obj = obj.split(",")
    if obj[0] == "newArr":
        clock.tick(1/(dt))
        pygame.display.update()
        screen.fill((255,255,255))
        continue
    try:
        obj = [float(o) for o in obj[:3]]
    except:
         pygame.quit()
         exit()
    pygame.draw.circle(screen, (0, 0, 0), (obj[0], obj[1]), obj[2])

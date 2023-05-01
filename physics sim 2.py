#n-body sim 2.0

import pygame
import numpy as np
import math
import random


xpos_index = 0
ypos_index = 1
xvel_index = 2
yvel_index = 3
xaccel_index = 4
yaccel_index = 5
xforce_index = 6
yforce_index = 7
rad_index = 8
mass_index = 9
G = 1


class Button:
    def __init(self, width, height, topleft, text):
        self.button = pygame.Surface(width, height)
        self.button.fill()


class Toolbar:
    def __init(self, width, height):
        self.image = pygame.Surface(width, height)
        self.image.fill((197, 229, 216))
        self.rect = self.image.get_rect()
        self.rect.topleft=(0, 0)


class Screen:
    def __init__(self, screen_dimensions = (1200, 950), center_cg = False):
        pygame.init()
        self.center_cg = center_cg
        self.screen_dimensions = screen_dimensions
        self.ball_list = np.empty((0, 0), np.float64)    # position(0, 1) --> velocity(2, 3) --> acceleration(4, 5)  --> force(6,7) --> radius(8) --> mass(9)
        self.screen = pygame.display.set_mode((self.screen_dimensions[0], self.screen_dimensions[1]))
        self.time = []
        self.num_obj = 0

    def update_num_balls(self):
        self.num_obj = self.ball_list.shape[0]        
        return self.num_obj

    def make_ball(self, position, velocity, accel, rad, mass):
        new_ball = np.array([position[0], position[1], velocity[0], velocity[1], accel[0], accel[1], rad, mass])
        self.ball_list = np.concatenate((self.ball_list, new_ball), axis=0)

    def get_mouse_pos(self):
        return pygame.mouse.get_pos()

    def get_dt(self):
        if len(self.time) > 0:
            if len(self.time) > 1:
                self.time.pop(0)
            self.time.append(pygame.time.get_ticks())
            return (self.time[1] - self.time[0])/1000
        else: 
            self.time.append(pygame.time.get_ticks())
            return 0

    def draw_ball(self):
        for i in range(self.num_obj):
            pygame.draw.circle(self.screen, (0, 0, 0), (self.ball_list[i][xpos_index], self.ball_list[i][ypos_index]), self.ball_list[i][rad_index])


    def update_forces(self):
        dt = self.get_dt()
        self.num_obj = self.update_num_balls()

        #reset forces and acceleration values to 0
        for i in range(int(self.num_obj)):
            for j in range(4, 8):
                self.ball_list[i][j] = 0


        for i in range(self.num_obj):   #calculating forces between all distinct pairs:
            for j in range(i + 1, self.num_obj):
                if i != j and j < self.num_obj: #if not comparing an object with itself
                    xdist = self.ball_list[j][xpos_index] - self.ball_list[i][xpos_index]
                    ydist = self.ball_list[j][ypos_index] - self.ball_list[i][ypos_index]
                    distance = math.sqrt(pow(xdist, 2) + pow(ydist, 2))
                    if distance > self.ball_list[i][rad_index] + self.ball_list[j][rad_index]:
                        #update forces between ball pairs
                        self.ball_list[i][xforce_index] += (xdist/distance)*G*self.ball_list[i][mass_index]*self.ball_list[j][mass_index]/pow(distance, 2)
                        self.ball_list[i][yforce_index] += (ydist/distance)*G*self.ball_list[i][mass_index]*self.ball_list[j][mass_index]/pow(distance, 2)

                        self.ball_list[j][xforce_index] += -(xdist/distance)*G*self.ball_list[i][mass_index]*self.ball_list[j][mass_index]/pow(distance, 2)
                        self.ball_list[j][yforce_index] += -(ydist/distance)*G*self.ball_list[i][mass_index]*self.ball_list[j][mass_index]/pow(distance, 2)

                        #update acceleration values
                        self.ball_list[i][xaccel_index] += self.ball_list[i][xforce_index]/self.ball_list[i][mass_index]
                        self.ball_list[i][yaccel_index] += self.ball_list[i][yforce_index]/self.ball_list[i][mass_index]

                        self.ball_list[j][xaccel_index] += self.ball_list[j][xforce_index]/self.ball_list[j][mass_index]
                        self.ball_list[j][yaccel_index] += self.ball_list[j][yforce_index]/self.ball_list[j][mass_index]

                        #update velocity values
                        self.ball_list[i][xvel_index] += self.ball_list[i][xaccel_index]*dt
                        self.ball_list[i][yvel_index] += self.ball_list[i][yaccel_index]*dt

                        self.ball_list[j][xvel_index] += self.ball_list[j][xaccel_index]*dt
                        self.ball_list[j][yvel_index] += self.ball_list[j][yaccel_index]*dt
            

                    else:  #handle collisions
                        self.ball_list[i][mass_index] += self.ball_list[j][mass_index]
                        #conservation of momentum
                        self.ball_list[i][xvel_index] = (self.ball_list[i][xvel_index]*self.ball_list[i][mass_index] + self.ball_list[j][xvel_index]*self.ball_list[j][mass_index])/(self.ball_list[i][mass_index]+self.ball_list[j][mass_index])
                        self.ball_list[i][yvel_index] = (self.ball_list[i][yvel_index]*self.ball_list[i][mass_index] + self.ball_list[j][yvel_index]*self.ball_list[j][mass_index])/(self.ball_list[i][mass_index]+self.ball_list[j][mass_index])
                        #conservation of mass
                        self.ball_list[i][xpos_index] = (self.ball_list[i][xpos_index]*self.ball_list[i][mass_index] + self.ball_list[j][xpos_index]*self.ball_list[j][mass_index])/(self.ball_list[i][mass_index]+self.ball_list[j][mass_index])
                        self.ball_list[i][ypos_index] = (self.ball_list[i][ypos_index]*self.ball_list[i][mass_index] + self.ball_list[j][ypos_index]*self.ball_list[j][mass_index])/(self.ball_list[i][mass_index]+self.ball_list[j][mass_index])
                        #conserving radii
                        self.ball_list[i][rad_index] = math.cbrt(pow(self.ball_list[i][rad_index], 3) + pow(self.ball_list[j][rad_index], 3))

                        self.ball_list = np.copy(self.ball_list)
                        self.ball_list = np.delete(self.ball_list, j, axis=0)
                        self.update_num_balls()

        self.num_obj = self.update_num_balls()
        for i in range(self.num_obj):
            self.ball_list[i][xpos_index] += self.ball_list[i][xvel_index]*dt
            self.ball_list[i][ypos_index] += self.ball_list[i][yvel_index]*dt


class Game:
    def __init__(self, init_balls, center_on_cg = False):
        self.init_balls = init_balls
        self.center_on_cg = center_on_cg
        self.screen = Screen()
        self.screen.update_num_balls()

    def create_obj(self, pos, xvel, yvel, rad):  # position(0, 1) --> velocity(2, 3) --> acceleration(4, 5)  --> force(6,7) --> radius(8) --> mass(9)
        if self.screen.num_obj < 1:
            self.screen.ball_list = np.array([[pos[0], pos[1], xvel, yvel, 0, 0, 0, 0, rad, pow(rad, 3)]], dtype=np.float64)
            self.screen.num_obj = self.screen.update_num_balls()
        else: 
            self.screen.ball_list = np.concatenate((self.screen.ball_list, np.array([[pos[0], pos[1], xvel, yvel, 0, 0, 0, 0, rad, pow(rad, 3)]])))
            self.screen.num_obj = self.screen.update_num_balls()

    def run(self):
        for i in range(80):
            self.create_obj((random.randrange(0, self.screen.screen_dimensions[0]), random.randrange(0, self.screen.screen_dimensions[1])), 0, 0, 3)
        while True:
            for event in pygame.event.get():
                if event.type == pygame.QUIT:
                    pygame.quit()
                if event.type == pygame.MOUSEBUTTONDOWN:
                    mouse_pos = pygame.mouse.get_pos()
                    if event.button == 1:
                        self.create_obj(mouse_pos, random.randrange(-100, 100), 0, 3)
                    if event.button == 3:
                        self.create_obj(mouse_pos, 0, 0, 10)
            self.screen.screen.fill((255, 255, 255))
            self.screen.update_forces()
            self.screen.draw_ball()
            pygame.display.update()

    


game = Game(0)

game.run()

                        

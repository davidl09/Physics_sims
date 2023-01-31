#three body problem pygame

import pygame
import numpy as np
import random

#global variables
screen_width = 1200
screen_height = 950

density = 100
last_time = 0
balls = []
times = []

class Object:
    def __init__(self, radius, density, position, velocity, is_stationary):
        self.is_stationary = is_stationary
        self.deleted = False
        self.radius = radius
        self.density = density
        self.position = position
        self.mass = self.density*pow(radius, 3)
        self.velocity = velocity
        self.position = self.position.astype(float)
        self.velocity = self.velocity.astype(float)
        self.force = np.array([[0, 0]]).astype(float)
        self.accel = np.array([[0, 0,]]).astype(float)
    
    def draw(self):
        pygame.draw.circle(screen, (0, 0, 0), (self.position[0][0], self.position[0][1]), self.radius)

    def delete(self):
        self.deleted = True

    def update(self):
        self.accel[0, 0] = self.force[0, 0]/self.mass
        self.accel[0, 1] = self.force[0, 1]/self.mass
        self.velocity += self.accel*dt
        self.position += self.velocity*dt
        

def previous_time():
    if len(times) > 1:
        times.pop(0)
    times.append(pygame.time.get_ticks())
    if len(times) == 1:
        return 0
    dt = (times[1] - times[0])
    return dt/1000

pygame.init()

time = pygame.time.Clock()

screen = pygame.display.set_mode((screen_width, screen_height))

last_time = pygame.time.get_ticks()

#balls.append(Object(15, 1000, np.array([[500, 400]]), np.array([[0, 0]]), True))
while True:
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            pygame.quit()
        if event.type == pygame.MOUSEBUTTONDOWN:
            if event.button == 1:
                mouse_pos = pygame.mouse.get_pos()
                balls.append(Object(3, density, np.array([[mouse_pos[0], mouse_pos[1]]]), np.array([[random.randrange(-100, 100), random.randrange(-100, 100)]]), False))
            if event.button == 3:
                mouse_pos = pygame.mouse.get_pos()
                balls.append(Object(9, density, np.array([[mouse_pos[0], mouse_pos[1]]]), np.array([[0, 0]]), False))

    screen.fill((255, 255, 255))
    dt = previous_time()
    

    for ball in balls:  #remove a ball from the master list if it has been deleted and reset all forces on balls to 0
        if ball.deleted:
            ball.pop()
        ball.force[0, 0] = 0
        ball.force[0, 1] = 0


    ball_pairs = []
    for i in range(len(balls)):
        for j in range(i+1, len(balls)):
            ball_pairs.append((balls[i], balls[j]))  #make a list of all possible combinations of balls (Cartesian product)
    for ball_pair in ball_pairs:  #get distance between all pairs
        distance = np.sqrt(np.power(ball_pair[0].position[0][0] - ball_pair[1].position[0][0], 2) + np.power(ball_pair[0].position[0][1] - ball_pair[1].position[0][1], 2)) 
    
        if distance <= ball_pair[0].radius + ball_pair[1].radius:  #handle ball collisions to conserve momentum, mass, volume, etc
            if ball_pair[0].mass > ball_pair[1].mass:
                i = 0
                j = 1
                
            else: 
                i = 1
                j = 0
            
            ball_pair[i].radius += pow(ball_pair[j].radius, 1/3)
            ball_pair[i].velocity = (ball_pair[i].velocity*ball_pair[i].mass+ball_pair[j].velocity*ball_pair[j].mass)/(ball_pair[i].mass + ball_pair[j].mass)
            ball_pair[i].mass += ball_pair[j].mass
            ball_pair[i].density = ball_pair[i].mass/pow(ball_pair[i].radius, 3)
            ball_pair[i].position = (ball_pair[i].position*ball_pair[i].mass+ball_pair[j].position*ball_pair[j].mass)/(ball_pair[i].mass+ball_pair[j].mass)
            ball_pair[j].delete()

            for i, ball_pair in enumerate(ball_pairs): #update list after any collisions
                if ball_pair[0].deleted == True or ball_pair[1].deleted == True:
                    ball_pairs.pop(i)
            for i, ball in enumerate(balls):
                if ball.deleted == True:
                    balls.pop(i)


        gravitational_force = (ball_pair[0].mass * ball_pair[1].mass)/(np.power(distance, 2))  #calculate x and y components of gravitational force between all pairs

        force_x1 = gravitational_force * (ball_pair[1].position[0][0] - ball_pair[0].position[0][0]) / distance
        force_y1 = gravitational_force * (ball_pair[1].position[0][1] - ball_pair[0].position[0][1]) / distance
        force_x2 = -force_x1
        force_y2 = -force_y1
        ball_pair[0].force[0, 0] += force_x1
        ball_pair[0].force[0, 1] += force_y1
        ball_pair[1].force[0, 0] += force_x2
        ball_pair[1].force[0, 1] += force_y2
        
    for ball in balls:  # update all ball positions, velocities, and accelerations 
        if not ball.deleted:
            if not ball.is_stationary:
                ball.draw()
                ball.update()
            else: ball.draw()
        
        
    
    pygame.display.update()
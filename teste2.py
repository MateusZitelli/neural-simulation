from random import random, choice
import pygame
from pygame.locals import *
from time import sleep
size = (200, 10)

class Neuron:
    def __init__(self,pos , type = 1):
        self.type = type
        self.pos = pos
        if type:
            self.a = 0.02
            self.b = 0.2
            var = random()
            self.c = -65 + 15 * var ** 2
            self.d = 8 - 6 * var ** 2
        else:
            var = random()
            self.a = 0.02 + 0.08 * var
            self.b = 0.25 - 0.05 * var
            self.c = -65
            self.d = 2
        self.v = -65
        self.u = self.b * self.v
        self.I = 0
        self.connected = []

    def run(self):
        if self.type:
            self.I = 5 * random()
        else:
            self.I = 2 * random()

    def update(self):
        self.v += 0.5 * (0.04 * self.v ** 2 + 5 * self.v + 150 - self.u + self.I)
        self.v += 0.5 * (0.04 * self.v ** 2 + 5 * self.v + 150 - self.u + self.I)
        self.u += self.a * (self.b * self.v - self.u)

    def send(self):
        for i in self.connected:
            i.I += 20

surface = pygame.display.set_mode((1000, 1000))
neurons = [Neuron((i % size[0], i / size[1]),random() > 0.1) for i in range(size[0] * size[1])]
for i in neurons:
    i.connected.append(neurons[i.pos[1] * size[0] + min(i.pos[0] + 1, size[1] - 1)])
    i.connected.append(neurons[min(i.pos[1] + 1, size[1] - 1) * size[0] + i.pos[0]])
    i.connected.append(neurons[max(i.pos[1] - 1, 0) * size[0] + i.pos[0]])
    i.connected.append(neurons[i.pos[1] * size[0] + max(i.pos[0] - 1, 0)])
    i.connected.append(neurons[max(i.pos[1] - 1, 0) * size[0] + min(i.pos[0] + 1, size[1] - 1)])
    i.connected.append(neurons[min(i.pos[1] + 1, size[1] - 1) * size[0] + min(i.pos[0] + 1, size[1] - 1)])
    i.connected.append(neurons[max(i.pos[1] - 1, 0) * size[0] + max(i.pos[0] - 1, 0)])
    i.connected.append(neurons[min(i.pos[1] + 1, size[1] - 1) * size[0] + max(i.pos[0] - 1, 0)])

last = (0,0)
for lo in range(10000000):
    for i in neurons:
        i.run()
    fired = filter(lambda x: x.v >= 30, neurons)
    for j, i in enumerate(fired):
        pygame.draw.line(surface, (255, 255, 255) , (i.pos[0], i.pos[1]), (i.pos[0], i.pos[1]))
        pygame.draw.line(surface, (255, 255, 255) , (lo % 1000, 400 + len(fired) / 10.0), last)
        last = (lo % 1000,  400 + len(fired) / 10.0)
        if lo % 1000 == 0:
            surface.fill((0,0,0))
            lo = 0
        i.v = i.c
        i.u += i.d
        i.send()
    for i in neurons:
        i.update()
    pygame.display.flip()
    surface.fill((0,0,0), (0,0, size[0], size[1]))

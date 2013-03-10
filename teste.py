from random import random
import pygame
from pygame.locals import *
from time import sleep
pulsos = []
Ne=50
Ni=200
re = [random() for i in range(Ne)]
ri= [random() for i in range(Ni)]
a=[0.02 for i in range(Ne)] + [0.02+0.08*ri[i] for i in range(Ni)]
b=[0.2 for i in range(Ne)] + [0.25-0.05*ri[i] for i in range(Ni)]
c=[-65 + 15 * re[i] ** 2 for i in range(Ne)] + [-65 for i in range(Ni)]
d=[8 - 6 * re[i] ** 2 for i in range(Ne)] + [2 for i in range(Ni)]
S=[[0.5 * random() for i in range(Ne)] for i in range(Ni + Ne)]+ [[-random() for i in range(Ni)] for i in range(Ni + Ne)]
v = [-65 for i in range(Ne + Ni)]
u = [b[i] * v[i] for i, j in enumerate(v)]
surface = pygame.display.set_mode((1000, 1000))
printados = []
for t in range(1000):
    print t
    I = [5 * random() for i in range(Ne)] + [2 * random() for i in range(Ni)]
    fired = [i for i,j in enumerate(v) if j >= 30]
    for i in fired:
        pulsos.append((t, i))
        v[i] = c[i]
        u[i] += d[i]
        for j, k in enumerate(S[:i]):
            I[j] += sum(k) * 0.6 + 2
    v = [i + 0.5 * (0.04 * i ** 2 + 5 * i + 140 - u[j] + I[j]) for j, i in enumerate(v)]
    v = [i + 0.5 * (0.04 * i ** 2 + 5 * i + 140 - u[j] + I[j]) for j, i in enumerate(v)]
    u = [j + a[i] * (b[i] * v[i] - j) for i, j in enumerate(u)]
for i in pulsos:
    pygame.draw.line(surface, (255, 255, 255), (i[0], 1000 - i[1]), (i[0], 1000 - i[1]))
pygame.display.flip()
sleep(100000000000)


        
    

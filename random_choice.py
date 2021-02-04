import random

week = random.randrange(1, 14)
problem = random.randrange(1, 6)

if problem == 5 :
    problem = 'PoTW'

print(f'{week}.{problem}')
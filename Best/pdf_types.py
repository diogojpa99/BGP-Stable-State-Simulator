import numpy as np
import matplotlib.pyplot as plt
import pandas as pd



data = np.loadtxt("types.txt")

print(data.shape)

index = [0,1,2,3]
types= []
types.append(data[0])
types.append(data[1])
types.append(data[2])
types.append(data[3])


#calcular o número total de custos
i=0
total=0

while i < len(types):
    total += types[i]
    i +=1

#probabilidade de cada indice
i=0
probs = []

while i < len(types):
    probs.append(types[i]/total)
    i +=1

print(index)
print(probs)

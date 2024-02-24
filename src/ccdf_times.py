import numpy as np
import matplotlib.pyplot as plt
import pandas as pd



data = np.loadtxt("times.txt")

print(data.shape)

index = []
times= []
times.append(data[0])

i = 0

while i < len(data):
    if data[i] != 0:
        index.append(i)
        times.append(data[i])
    i +=1 

#calcular o número total de custos
i=0
total=0

while i < len(times):
    total += times[i]
    i +=1

#probabilidade de cada indice
i=0
probs = []

while i < len(times):
    probs.append(times[i]/total)
    i +=1

#cdf
i=0
j=0
cdf_aux=0

cdf =[]
while i < len(probs):
    j=0
    cdf_aux=0
    while j <= i:
        cdf_aux += probs[j]
        j+=1
    cdf.append(cdf_aux)
    i +=1


#ccdf
ccdf=[]

i=0

while i < len(cdf):
    ccdf.append(1-cdf[i])
    i +=1

#plot
print(index)
print(times)
print(ccdf)


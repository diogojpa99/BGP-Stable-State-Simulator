import numpy as np
import matplotlib.pyplot as plt
import pandas as pd



data = np.loadtxt("costs_algorithm.txt")

print(data.shape)

costs = []
costs.append(data[0])

i = 1 
flag = 0

while flag != 1:

    if data[i] == 0:
        flag = 1
    else:
        costs.append(data[i])
    i +=1

#calcular o número total de custos
i=0
total=0

while i < len(costs):
    total += costs[i]
    i +=1

#probabilidade de cada indice
i=0
probs = []

while i < len(costs):
    probs.append(costs[i]/total)
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
print(costs)
print(ccdf)

#indices
i=0
indices=[]
while i < len(costs):
    indices.append(i)
    i +=1


plt.scatter(indices,ccdf)
plt.show()

x = range(len(ccdf))
plt.stem(x, ccdf)
plt.xticks(x, indices)
plt.axis([-0.1, len(indices), -0.001, 1.05])
plt.show()


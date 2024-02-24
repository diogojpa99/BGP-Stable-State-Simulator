import numpy as np
import matplotlib.pyplot as plt
import pandas as pd



data = np.loadtxt("types_costs_algorithm.txt")

types=data[:,0]
costs=data[:,1]

i=0
index_zeros=[]
index_infinite=[]

while i< len(types):
    if types[i] == 0:
        index_zeros.append(i)
    if costs[i] == 1000000:
        index_infinite.append(i)
    i +=1

print(index_infinite)
print(index_zeros)

types=pd.DataFrame(types)
costs=pd.DataFrame(costs)

#Remover zeros do types
types=types.drop(index_zeros)

#Remover zeros e infinites do cost
costs=costs.drop(index_zeros)
costs=costs.drop(index_infinite)

#Já temos os vetores limpos
types = types.to_numpy()
costs = costs.to_numpy()

print(costs.max(), costs.min())

'''
count, bins_count = np.histogram(types, bins=20, range=(types.min(),types.max()))
  
# finding the PDF of the histogram using count values
pdf = count / sum(count)
  
# using numpy np.cumsum to calculate the CDF
# We can also find using the PDF values by looping and adding
cdf = np.cumsum(pdf)
  
# plotting PDF and CDF
#plt.plot(bins_count[1:], pdf, color="red", label="PDF")
plt.plot(bins_count[1:], 1 - cdf, label="CDF")
plt.show()
plt.legend()

'''
'''
costs = np.sort(costs)
y = 1 - np.arange(1, len(costs)+1) / len(costs)
plt.plot(costs,y)
plt.margins(0.02)
plt.show()

plt.hist(costs)
plt.show()

plt.hist(types)
plt.show()
'''

'''
types = np.sort(types)
y = np.arange(1, len(types)+1) / len(types)
plt.plot(types,y)
plt.margins(0.02)
plt.show()
'''
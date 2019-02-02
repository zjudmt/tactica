import os
import numpy as np
from sklearn.cluster import KMeans,MeanShift, estimate_bandwidth
from sklearn.decomposition import PCA
import pandas as pd
import matplotlib.pyplot as plt

'''functions'''
def analyze(data, step = 250):
	for i in range(data[0,0], data[-1,0], step):
		start = i
		end = i + step
		# print("start:", start, "end", end)
		match = data[np.where( (data[:,0]>start)*(data[:,0]<end) )]
		counts = np.bincount(match[:,1], minlength = 8)
		counts = counts.reshape((1,-1))
		try:
			aggregation = np.vstack((aggregation, counts))		
		except Exception as e:
			aggregation = counts
		# print("counts", counts)
	print(aggregation)
	x = [i for i in range(data[0,0],  data[-1,0], step) ]
	for i in range(0, aggregation.shape[1]):
		plt.plot(x, aggregation[:,i], label = 'cluster ' + str(i+1) )
	plt.legend()
	plt.title('clusters')
	plt.show()
	pass


times = np.loadtxt("results/time_stamp.txt")
times = times.reshape((77440,1))
print(times.shape)

clusters = pd.read_excel("results/kmeans.xlsx")
# print(clusters.loc[1])
print(clusters.index.shape)
indices = np.array(clusters.index).reshape((-1,1))

data = np.hstack((times, indices)).astype('int')
data = data[np.where( (data[:,0]>16132)*(data[:,0]<22974) )]
analyze(data)

# a = np.array([[ 51, 272, 194, 118, 79, 0, 56, 229]])
# print(a.shape)
# b = np.array([[157, 7, 165, 44, 75, 0, 74, 364]])
# c = np.vstack((a,b))
# print(c)


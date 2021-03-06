import os
import numpy as np
from sklearn.cluster import KMeans,MeanShift, estimate_bandwidth
from sklearn.decomposition import PCA
import pandas as pd
import matplotlib.pyplot as plt

'''parameters'''
# 以帧数为单位的步长
aggregation_step = 250
start_frame = 16132
end_frame = 22974

'''function'''
def analyze(data, step = 250):
	for i in range(data[0,0], data[-1,0], step):
		start = i
		end = i + step
		match = data[np.where( (data[:,0]>start)*(data[:,0]<end) )]
		counts = np.bincount(match[:,1], minlength = 8)
		counts = counts.reshape((1,-1)) / match.shape[0]
		try:
			aggregation = np.vstack((aggregation, counts))		
		except Exception as e:
			aggregation = counts
	print(aggregation)
	x = [i for i in range(data[0,0],  data[-1,0], step) ]
	for i in range(0, aggregation.shape[1]):
		plt.plot(x, aggregation[:,i], label = 'cluster ' + str(i+1) )
	plt.legend()
	plt.title('clusters')
	plt.show()
	pass

# 载入时间戳数据
times = np.loadtxt("results/time_stamp.txt")
times = times.reshape((77440,1))
print(times.shape)

# 载入聚类结果
clusters = pd.read_excel("results/kmeans.xlsx")
indices = np.array(clusters.index).reshape((-1,1))

# 合并数据
data = np.hstack((times, indices)).astype('int')
data = data[np.where( (data[:,0]>start_frame)*(data[:,0]<end_frame) )]

# 调用函数分析数据
analyze(data, aggregation_step)


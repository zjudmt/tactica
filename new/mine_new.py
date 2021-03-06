import os
import numpy as np
from sklearn.cluster import KMeans,MeanShift, estimate_bandwidth
from sklearn.decomposition import PCA
import pandas as pd
import matplotlib.pyplot as plt

'''设定参数'''
# PCA降维维度
pca_dimensions = 2
# K-Means聚类数
n_clusters = 8
# 聚类完成后储存文件位置
cluster_path = 'results/kmeans.xlsx'

'''程序入口'''
# 读入数据
data = np.loadtxt("results/output.txt")
data = data.reshape((77440, 2*2*8))
print("load success")

pca = PCA(n_components = pca_dimensions)
lo_data = pca.fit_transform(data)
# 输出pca结果对于方差的比例
print(pca.explained_variance_ratio_)
print("pca success", lo_data.shape)

# 建立模型
# model = MeanShift(n_jobs = -2, bin_seeding = True)
model = KMeans(n_clusters = n_clusters, random_state = 0)
print("model success")

# 应用模型
model.fit(lo_data)
print("fit success")

col = ['x' + str(i) for i in range(0, 2) ]
#labels为分类的标签
labels = model.labels_
clusters = dict(zip(*np.unique(labels, return_counts=True)))
print("stat success", clusters)

'''绘制聚类图'''
colors = [['red','green','blue','grey','yellow','black','cyan','magenta'][label] for label in labels]
plt.scatter(lo_data[:,0],lo_data[:,1],c=colors,s=10)
plt.title('KMeans')
plt.show()

#把标签加入到矩阵中用DataFrame生成新的df，index为类别的编号，这里是0,1,2
df = pd.DataFrame(lo_data,index=labels,columns=col)
#数据保存在excel文件中
writer = pd.ExcelWriter(cluster_path)
# writer = pd.ExcelWriter('results/meanshift.xlsx')
df.to_excel(writer,'page_1',float_format='%.5f') # float_format 控制精度
writer.save()

print("save success")


import os
import numpy as np
from sklearn.cluster import KMeans
from sklearn.decomposition import PCA
import pandas as pd


data = np.loadtxt("results/output.txt")
data = data.reshape((77440, 2*2*8))
print(data.shape)
# df = pd.DataFrame(data)

print("load success")
pca = PCA(n_components = 2)
lo_data = pca.fit_transform(data)

print("pca success", lo_data.shape)
kmeans = KMeans(n_clusters = 10, random_state = 0).fit(lo_data)
col = ['x' + str(i) for i in range(0, 2) ]
#labels为分类的标签
labels = kmeans.labels_
#把标签加入到矩阵中用DataFrame生成新的df，index为类别的编号，这里是0,1,2
df = pd.DataFrame(lo_data,index=labels,columns=col)
#数据保存在excel文件中
writer = pd.ExcelWriter('results/cluster.xlsx')
df.to_excel(writer,'page_1',float_format='%.5f') # float_format 控制精度
writer.save()
# dataDf.to_excel('results/cluster.xls')

print("kmeans success")
print(pca.explained_variance_ratio_)

简介处理流程

第一步：将一场比赛的事件流分成多个阶段
第二步：将阶段按时空成分聚类
第三步：聚类排序
第四步：挖掘模式
第五步：模式排序


我们前三步使用的是C++，后两步使用的是Python（前期数据处理代码没有放上来，是用的JavaScript）

tactica.cpp处理第一步(main函数在tactica.cpp里)
cluster.cpp处理第二三步
生成聚类之后会将不同的聚类放在不同文件里，放在Cluster文件夹里
然后mine.py将读取这些文件并使用PrefixSpan算法进行序列模式挖掘

cmd.txt是方便我使用命令行的g++编译时的命令，主要命令是
g++ Cluster.cpp tactica.cpp --std=c++11 -o tactica


https://github.com/chuanconggao/PrefixSpan-py
from prefixspan import PrefixSpan


def loadFile(filepath):
    "从指定文件名载入数据到列表"
    db = []
    with open(filepath) as file:
        line = file.readline()
        while line:
            db.append(eval(line))
            line = file.readline()
        
    return db


def generateFilename(tno,cno):
    "根据队编号与聚类编号产生文件名"
    return "Team"+str(tno)+"Cluster"+str(cno)+".txt";

    
path = "Cluster/"
for tno in range(1,3):
    for cno in range(0,5):
        filepath = path+generateFilename(tno,cno)
#        filepath="C:/Users/syby119/Desktop/test.txt"
        db = loadFile(filepath)
        ps = PrefixSpan(db)
        # 输出频数大于等于1的序列（无序）
#        print(ps.frequent(1))
        # 输出频数前5的序列（降序）
        print(ps.topk(10))
        print("\n")

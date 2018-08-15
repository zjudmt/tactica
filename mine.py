from prefixspan import PrefixSpan

w_shot = 2
w_pass = 0.5
w_other = 1

def getweight(e):
    if e < 10 :
        return w_other
    elif e < 35:
        return w_pass
    else:
        return w_shot
    
# 模式的类
class Pattern:
    def __init__(self, freq, flow):
        super(Pattern, self).__init__()
        self.freq = freq
        self.flow = flow
        self.score = self.score()

    # 在构造函数中直接计算出该模式的Score方便排序
    def score(self):
        event_value = 0
        for x in self.flow:
            event_value += getweight(x)
        return self.freq * event_value


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
list_p = []
# tno 代表 team number
# cno 代表 cluster number
for tno in range(1,3):
    for cno in range(0,5):
        filepath = path+generateFilename(tno,cno)
        db = loadFile(filepath)
        ps = PrefixSpan(db)
        for x in range(0,10):
            list_p.append( Pattern(ps.topk(10)[x][0], ps.topk(10)[x][1]) )
        # 输出当前Cluster中出现频率最高的10个Pattern
        # print(ps.topk(10))

# 对Score进行排序
list_p = sorted(list_p, key=lambda x:x.score, reverse=True)
print("#######################################")
for x in list_p:
    print("score:", x.score, "freq:", x.freq)
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
    

class Pattern:
    """docstring for Pattern"""
    def __init__(self, freq, flow):
        super(Pattern, self).__init__()
        self.freq = freq
        self.flow = flow
        self.score = self.score()


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
for tno in range(1,3):
    for cno in range(0,5):
        filepath = path+generateFilename(tno,cno)
        db = loadFile(filepath)
        ps = PrefixSpan(db)
        # 输出频数大于等于1的序列（无序）
#        print(ps.frequent(1))
        # 输出频数前5的序列（降序）
        for x in range(0,10):
            list_p.append( Pattern(ps.topk(10)[x][0], ps.topk(10)[x][1]) )
        # print(ps.topk(10))
        # print("\n")

list_p = sorted(list_p, key=lambda x:x.score, reverse=True)
print("#######################################")
for x in list_p:
    print("score:", x.score, "freq:", x.freq)
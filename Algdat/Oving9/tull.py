from sys import stdin

Inf = float(1e3000)

class edge:
    def __init__(self, price, frm, to):
        self.price =price
        self.frm = frm
        self.to = to 
    def __lt__(self, rhs):
        return self.price < rhs.price
    def __str__(self):
        p = str(self.price)
        f = str(self.frm)
        t = str(self.to)
        return "price: "+p+" To: "+t+" From: "+f

def cleanReps(to,  reps):

    L = []
    current = to
    while current != reps[current]:
        L.append(current)
        current= reps[current]
    for i in L:
        reps[i] = current



lines = [line.strip() for line in stdin]
length = len(lines)
reps = [i for i in range(length)]

edges = []
counter =0
for line in lines:
    edg = line.split()
    for frTo in edg:
        to, price = frTo.split(":")
        to = int(to)
        price = int(price)
        if counter > to:
            edges.append(edge(price, counter, to)) 
    counter+=1
# for i in edges:
#     print(i)    
edges.sort()

most_expensive =0
connectedEdges =0
for i in edges:
    cleanReps(i.to, reps)
    cleanReps(i.frm, reps)
    if reps[i.to] != reps[i.frm]:
        reps[i.frm]= reps[i.to]
        connectedEdges+=1
        most_expensive= i.price
if connectedEdges< length-1:
    print(Inf)

if length <=1:
    print(0)
    quit()
else:
    print(most_expensive)


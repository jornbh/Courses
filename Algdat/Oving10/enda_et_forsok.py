
from sys import stdin, stderr

class frac:
    def __init__(self, string):
        

        parts = string.split(".")
        if len(parts)< 2:
            self.nominator = int(parts[0])
            self.denominator =1
            return
        self.nominator = int(parts[0]+parts[1])
        self.denominator = 10**(len(parts[1]))

    def __lt__(self, rhs):
        return rhs.denominator * self.nominator < rhs.nominator*self.denominator
    def __mul__(self, rhs):
        nom = self.nominator* rhs.nominator
        den = self.denominator * rhs.denominator
        output = frac("1")
        #In case of crashing
        if nom >10000000 or den > 100000000:
            nom /= 10000
            den /= 10000
        #end 
        
        output.nominator = nom
        output.denominator = den
        return output
    def __str__(self):
        return "("+ str(self.nominator)+ "/" + str(self.denominator)+")"


class Node:
    def __init__(self, id, prob):
        self.id = id
        self.prob = prob

    def __lt__(self, rhs):
        return self.prob < rhs.prob
    def __str__(self):
        i = str(self.id)
        prb = self.prob.__str__()
        return ("("+i +" ," +prb +")")



def getInfo():
    n = int(stdin.readline())
    probabilities = [frac(x) for x in stdin.readline().split()]
    neighbour_matrix = []
    neighboursList=[]
    for line in stdin:
        neighbour_row = [0] * n
        neighbours = [int(neighbour) for neighbour in line.split()]
        for neighbour in neighbours:
            neighbour_row[neighbour] = 1
        neighbour_matrix.append(neighbour_row)
        neighboursList.append(neighbours)
    return neighboursList, probabilities




class heap:
    def __init__(self, size):
        self.heap =[None]
        self.positions = [-1 for i in range(size)]
    
    def __str__(self):
        outputList =[]
        for i in self.heap[:]:
            outputList.append(i.__str__())
        return ", ".join(outputList)




    def push(self, node):
        ID = node.id
        if self.positions[ID] == -1:  
            self.heap.append(node)
            self.positions[ID] = len(self.heap)-1
            self.fixUpwards(len(self.heap)-1)
        elif self.positions ==0:
            print("Node has already been through the heap")
        else:
            if node <  self.heap[self.positions[ID]]:
                return
            self.heap[self.positions[ID]] = node
            self.fixUpwards(self.positions[ID])
    
    def fixUpwards(self, fixIndex):
        index = fixIndex
        length = len(self.heap)
        while index> 1:
            parentIndex = index//2
            if self.heap[index] > self.heap[parentIndex]:
                self.heap[index], self.heap[parentIndex] = self.heap[parentIndex] , self.heap[index]
                self.positions[self.heap[index].id] = index
            else:
                break
            index = parentIndex
        self.positions[self.heap[index].id] = index



    def pop(self):
        output = self.heap[1]
        self.heap[1] = self.heap[-1]
        self.heap.pop()
        self.fixDownwards(1)
        return output
    def __len__(self):
        return len(self.heap)-1
    def fixDownwards(self, index):
        length = len(self.heap)
        if length == 1:
            return
        leftChild = 2*index
        rightChild = leftChild+1
        while rightChild< length:
            if self.heap[leftChild] > self.heap[rightChild]:
                swapIndex = leftChild
            else:
                swapIndex = rightChild
            if self.heap[index] < self.heap[swapIndex]:
                self.heap[index], self.heap[swapIndex] = self.heap[swapIndex], self.heap[index]
                self.positions[self.heap[index].id] = index
            index = swapIndex
            leftChild = 2*index
            rightChild = leftChild+1
        
        if leftChild < length:
            if self.heap[index]< self.heap[leftChild]:
                self.heap[index], self.heap[leftChild]= self.heap[leftChild], self.heap[index]
                self.positions[self.heap[leftChild].id] = leftChild
        self.positions[self.heap[index].id] = index




def best_path(neighbors, prob):
    zeroFrac = frac("0")
    cP= [ zeroFrac for i in prob]
    parents =[-1 for i in prob]
    cP[0] = prob[0]
    h = heap(len(prob))
    h.push(Node(0, prob[0]))
    endIndex = len(neighbors)-1
    while(len(h)):

        N = h.pop()

        for i in neighbors[N.id]:
            if cP[i] < N.prob*prob[i]:
                cP[i] = N.prob*prob[i]
                h.push(Node(i,cP[i]))
                parents[i] = N.id
    

    itr = endIndex
    if parents[-1] == -1:
        return "0"
    Lo = []
    while itr!= 0:
        Lo.append(itr)
        itr = parents[itr]
    Lo.append(0)
    return("-".join([str(i) for i in reversed(Lo)]))







def main():
    nL, prob = getInfo()
    print(best_path(nL, prob))

main()
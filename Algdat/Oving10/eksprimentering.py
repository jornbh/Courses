from sys import stdin, stderr

def push(h,p, node):
        ID = node[0]
        if p[ID] == -1:  
            h.append(node)
            p[ID] = len(h)-1
            fixUpwards(h,p,len(h)-1)
        elif p ==0:
            print("Node has already been through the heap")
        else:
            if node[1] <  h[p[ID]][1]:
                return
            h[p[ID]] = node
            fixUpwards(h,p,p[ID])
    
def fixUpwards(h,p, fixIndex):
    index = fixIndex
    length = len(h)
    while index> 1:
        parentIndex = index//2
        if h[index][1] > h[parentIndex][1]:
            h[index], h[parentIndex] = h[parentIndex] , h[index]
            p[h[index][0]] = index
        else:
            break
        index = parentIndex
    p[h[index][0]] = index



def pop(h,p):
    output = h[1]
    h[1] = h[-1]
    h.pop()
    fixDownwards(h,p,1)
    return output

def fixDownwards(h,p, index):
    length = len(h)
    if length == 1:
        return
    leftChild = 2*index
    rightChild = leftChild+1
    while rightChild< length:
        if h[leftChild][1] > h[rightChild][1]:
            swapIndex = leftChild
        else:
            swapIndex = rightChild
        if h[index][1] < h[swapIndex][1]:
            h[index], h[swapIndex] = h[swapIndex], h[index]
            p[h[index][0]] = index
        index = swapIndex
        leftChild = 2*index
        rightChild = leftChild+1
    
    if leftChild < length:
        if h[index][1]< h[leftChild][1]:
            h[index], h[leftChild]= h[leftChild], h[index]
            p[h[leftChild][0]] = leftChild
    p[h[index][0]] = index







###################################################
def best_path(neighbours, prob):
    cp = [0 for i in prob]
    pa = [-1 for i in prob]
    cp[0] = prob[0]
    hp = [None]
    pos = [-1 for i in prob]
    pos[0] = len(hp)
    hp.append((0, cp[0])) 
    end = len(prob)-1
    while len(hp) >1:
        CN = pop(hp,pos)
        
        for i in neighbours[CN[0]]:
            if cp[i] < CN[1]*prob[i]:
                cp[i] = CN[1]*prob[i]
                pa[i] = CN[0]
                if pos[i] ==-1:
                    push(hp, pos, (i,cp[i]))
                else:
                    hp[pos[i]][1] = cp[i]
                    fixUpwards(hp, pos, pos[i])
        if CN[0] == end:
            break
    
    if cp[-1] == 0:
        return "0"

    # Itterate backwards to path
    L = []
    itr = end
    while itr != 0:
        L.append(itr)
        itr = pa[itr]
        if itr == -1:
            quit()
    L.append(0)
    return "-".join( str(i) for i in (reversed(L)))



    
    

def main():
    n = int(stdin.readline())
    probabilities = [float(x) for x in stdin.readline().split()]
    neighboursList=[]
    for line in stdin:
        neighboursList.append([int(neighbour) for neighbour in line.split()])
    print (best_path(neighboursList, probabilities))



if __name__ == '__main__':
    main()
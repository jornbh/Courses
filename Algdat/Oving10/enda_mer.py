from sys import stdin
import queue





###################################################
def best_path(neighbours, prob):
    q = queue.PriorityQueue()
    cp = [0 for i in prob]
    pa = [-1 for i in prob]
    cp[0] = prob[0]
    q.put((-cp[0],0))
    end = len(prob)-1
    found = [False for i in prob]
    while not q.empty():
        CN = q.get()
        if found[CN[1]]:
            continue
        found[CN[1]] = True
        for i in neighbours[CN[1]]:
            if cp[i] < -CN[0]*prob[i]:
                cp[i] = -CN[0]*prob[i]
                pa[i] = CN[1]
                if not found[i]:
                    q.put((-cp[i], i))
        if CN[1] == end:
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
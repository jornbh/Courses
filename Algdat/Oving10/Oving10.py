
from sys import stdin, stderr

def best_path(nm, prob):
    currentProb = [0 for i in nm]
    currentProb[0] = prob[0]
    parents = [-1 for i in prob]
    changed = True 
    while changed:
        changed = False
        for i in range(len(nm)):
            for j in range(len(nm)):
                if 1==nm[i][j]:
                    if currentProb[j] < currentProb[i]*prob[j]:
                        currentProb[j] = currentProb[i]*prob[j]
                        parents[j] = i
                        changed = True


    curr = len(parents)-1
    outputList = [curr]
    if currentProb[-1] == 0:
        return "0"
    
    while curr!=0:
        curr = parents[curr]
        outputList.append(curr)



    #print(currentProb[-1])
    return "-".join([ str(i) for i in reversed(outputList)])


def main():
    n = int(stdin.readline())
    probabilities = [float(x) for x in stdin.readline().split()]
    neighbour_matrix = []
    for line in stdin:
        neighbour_row = [0] * n
        neighbours = [int(neighbour) for neighbour in line.split()]
        for neighbour in neighbours:
            neighbour_row[neighbour] = 1
        neighbour_matrix.append(neighbour_row)
    print(best_path(neighbour_matrix, probabilities))


if __name__ == "__main__":
    main()
